/***********************************************************************************************
 *                                                                                             *
 * This file is part of MailUnit.                                                              *
 *                                                                                             *
 * MailUnit is free software: you can redistribute it and/or modify it under the terms of      *
 * the GNU General Public License as published by the Free Software Foundation,                *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * MailUnit is distributed in the hope that it will be useful,  but WITHOUT ANY WARRANTY;      *
 * without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  *
 * See the GNU General Public License for more details.                                        *
 *                                                                                             *
 * You should have received a copy of the GNU General Public License along with MailUnit.      *
 * If not, see <http://www.gnu.org/licenses/>.                                                 *
 *                                                                                             *
 ***********************************************************************************************/

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_FUSION_DONT_USE_PREPROCESSED_FILES
#define BOOST_MPL_LIMIT_VECTOR_SIZE 30 // Max count of the state machine's rows
#define FUSION_MAX_VECTOR_SIZE 20 // Max count of the state machine's states

#include <boost/optional.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/back/state_machine.hpp>
#include <boost/scope_exit.hpp>
#include <MailUnit/Exception.h>
#include <MailUnit/Logger.h>
#include <MailUnit/Smtp/Protocol.h>

#define VERB_EHLO     "EHLO"
#define VERB_MAIL     "MAIL"
#define VERB_RCPT     "RCPT"
#define VERB_DATA     "DATA"
#define VERB_QUIT     "QUIT"
#define VERB_STARTTLS "STARTTLS"
#define END_OF_DATA   "\r\n.\r\n"

using namespace MailUnit;
using namespace MailUnit::Storage;
using namespace MailUnit::Smtp;
using boost::msm::front::Row;
using boost::msm::front::none;

namespace {

class ProtocolException : public Excpetion
{
public:
    ProtocolException(Response _response, const std::string & _message) :
        Excpetion(_message),
        m_response(_response)
    {
    }

    const Response & response() const
    {
        return m_response;
    }

private:
    Response m_response;
}; // class ProtocolException

enum class InputMode
{
    verb,
    raw
}; // enum class InputMode

class EventBase
{
protected:
    EventBase(const char * _data, std::size_t _data_lenght, Storage::RawEmail & _email) noexcept :
        mp_data(_data),
        m_data_lenght(_data_lenght),
        mr_email(_email)
    {
    }

    virtual ~EventBase()
    {
    }

public:
    EventBase(const EventBase &) = default;

    EventBase & operator = (const EventBase &) = default;

    Storage::RawEmail & email() const noexcept
    {
        return mr_email;
    }

    const char * data() const noexcept
    {
        return mp_data;
    }

    std::size_t dataLenght() const noexcept
    {
        return m_data_lenght;
    }

private:
    const char * mp_data;
    std::size_t m_data_lenght;
    Storage::RawEmail & mr_email;
}; // class EventBase

enum class EventId
{
    ready      = 0,
    ehlo       = 1,
    startTls   = 2,
    mailFrom   = 3,
    rcptTo     = 4,
    dataHeader = 5,
    data       = 6,
    quit       = 7
}; // enum class EventId

template<EventId id>
class Event : public EventBase
{
public:
    Event(const char * _data, std::size_t _data_lenght, Storage::RawEmail & _email) noexcept :
        EventBase(_data, _data_lenght, _email)
    {
    }
}; // class Event

using ReadyEvent      = Event<EventId::ready>;
using EhloEvent       = Event<EventId::ehlo>;
using StartTlsEvent   = Event<EventId::startTls>;
using MailFromEvent   = Event<EventId::mailFrom>;
using RcptToEvent     = Event<EventId::rcptTo>;
using DataHeaderEvent = Event<EventId::dataHeader>;
using RawDataEvent    = Event<EventId::data>;
using QuitEvent       = Event<EventId::quit>;

enum class StateId
{
    start      = 0,
    ready      = 1,
    ehlo       = 2,
    startTls   = 3,
    mailFrom   = 4,
    rcptTo     = 5,
    dataHeader = 6,
    data       = 7,
    quit       = 8
}; // enum class StateId

template<StateId id>
class State : public boost::msm::front::state<>
{
public:
    virtual ~State()
    {
    }
}; // class State

using StartState      = State<StateId::start>;
using ReadyState      = State<StateId::ready>;
using EhloState       = State<StateId::ehlo>;
using StartTlsState   = State<StateId::startTls>;
using MailFromState   = State<StateId::mailFrom>;
using RcptToState     = State<StateId::rcptTo>;
using DataHeaderState = State<StateId::dataHeader>;
using QuitState       = State<StateId::quit>;

class DataState : public State<StateId::data>
{
public:
    DataState()
    {
        resetTail();
    }

    void resetTail(const char * _data, std::size_t _data_length)
    {
        resetTail();
        m_tail_length = std::min(s_tail_size - 1, _data_length);
        strncpy(mp_tail, &_data[_data_length - m_tail_length], m_tail_length);
    }

    void resetTail()
    {
        m_tail_length = 0;
        memset(mp_tail, 0, s_tail_size);
    }

    const char * tail() const
    {
        return mp_tail;
    }

    std::size_t tailLength() const
    {
        return m_tail_length;
    }

private:
    static const std::size_t s_tail_size = sizeof(END_OF_DATA);
    char mp_tail[s_tail_size];
    std::size_t m_tail_length;
}; // class DataState

class ProtocolController
{
public:
    ProtocolController(Repository & _repositry, ProtocolTransport & _transport);

    virtual ~ProtocolController()
    {
    }

    const std::vector<const ProtocolExtenstion *> & extensions() const
    {
        return m_extensions;
    }

    void registerExtenstion(ProtocolExtenstionId _id);

    void unregisterExtenstion(ProtocolExtenstionId _id);

    void setInputMode(InputMode _mode)
    {
        if(_mode != m_mode)
        {
            m_mode = _mode;
        }
    }

    InputMode inputMode() const
    {
        return m_mode;
    }

    RawEmail & rawEmail()
    {
        return *m_current_email_ptr;
    }

    void writeResponse(const Response & _response)
    {
        mr_transport.addNextAction([this, _response]() {
            mr_transport.requestForWrite(_response);
        });
    }

    void listen()
    {
        mr_transport.addNextAction([this]() {
            mr_transport.requestForRead();
        });
    }

    void switchToTls()
    {
        mr_transport.addNextAction([this]() {
            mr_transport.requestForSwitchToTls();
        });
    }

    void storeEmail()
    {
        mr_repositry.storeEmail(*m_current_email_ptr);
        m_current_email_ptr = mr_repositry.createRawEmail();
    }

    void quit()
    {
        mr_transport.addNextAction([this]() {
            mr_transport.requestForExit();
        });
    }

protected:
    ProtocolTransport & transport()
    {
        return mr_transport;
    }

private:
    Repository & mr_repositry;
    ProtocolTransport & mr_transport;
    std::unique_ptr<RawEmail> m_current_email_ptr;
    InputMode m_mode;
    std::vector<const ProtocolExtenstion *> m_extensions;
}; // ProtocolController

ProtocolController::ProtocolController(Repository & _repositry, ProtocolTransport & _transport) :
    mr_repositry(_repositry),
    mr_transport(_transport),
    m_current_email_ptr(_repositry.createRawEmail()),
    m_mode(InputMode::verb)
{
}

void ProtocolController::registerExtenstion(ProtocolExtenstionId _id)
{
    bool already_registered = m_extensions.end() != std::find_if(m_extensions.begin(), m_extensions.end(),
        [_id](const ProtocolExtenstion * ext) {
            return ext->id() == _id;
        });
    if(already_registered)
    {
        return;
    }
    switch(_id)
    {
    case ProtocolExtenstionId::startTls:
        m_extensions.push_back(new StartTlsProtocolExtenstion());
        break;
    default:
        LOG_ERROR << "Unknown protocol extenstion id: " << static_cast<int>(_id);
        break;
    }
}

void ProtocolController::unregisterExtenstion(ProtocolExtenstionId _id)
{
    auto ext_it = std::find_if(m_extensions.begin(), m_extensions.end(), [_id](const ProtocolExtenstion * ext) {
        return ext->id() == _id;
    });
    if(m_extensions.end() != ext_it)
    {
        delete *ext_it;
        m_extensions.erase(ext_it);
    }
}

class ReadyAction
{
public:
    template<typename SourceStateT, typename TargetStateT>
    void operator ()(const EventBase &, ProtocolController & _protocol, SourceStateT &, TargetStateT &)
    {
        _protocol.writeResponse(ResponseCode::ready);
        _protocol.listen();
    }
}; // class ReadyAction

class EhloAction
{
public:
    template<typename SourceStateT, typename TargetStateT>
    void operator ()(const EventBase &, ProtocolController & _protocol, SourceStateT &, TargetStateT &)
    {
        Response response(ResponseCode::ok);
        for(const ProtocolExtenstion * ext : _protocol.extensions())
            response.addExtenstion(*ext);
        _protocol.writeResponse(response);
        _protocol.listen();
    }
}; // class EhloAction

class StartTlsAction
{
public:
    template<typename SourceStateT, typename TargetStateT>
    void operator ()(const EventBase &, ProtocolController & _protocol, SourceStateT &, TargetStateT &)
    {
        _protocol.unregisterExtenstion(ProtocolExtenstionId::startTls);
        _protocol.writeResponse(ResponseCode::ready);
        _protocol.switchToTls();
        _protocol.listen();
    }
}; // class StartTlsAction

class StartTlsGuard
{
public:
    template<typename SourceStateT, typename TargetStateT>
    bool operator ()(const EventBase &, ProtocolController & _protocol, SourceStateT &, TargetStateT &)
    {
        const std::vector<const ProtocolExtenstion *> & extensions = _protocol.extensions();
        return extensions.end() != std::find_if(extensions.begin(), extensions.end(),
            [](const ProtocolExtenstion * ext) {
                return ext->id() == ProtocolExtenstionId::startTls;
            });
    }
}; // class StartTlsGuard

class MailFromAction
{
public:
    template<typename SourceStateT, typename TargetStateT>
    void operator ()(const EventBase & _event, ProtocolController & _protocol, SourceStateT &, TargetStateT &);

private:
    static const std::size_t s_cmd_length = sizeof(VERB_MAIL " FROM:") - 1;
}; // class MailFromAction

template<typename SourceStateT, typename TargetStateT>
void MailFromAction::operator ()(const EventBase & _event, ProtocolController & _protocol, SourceStateT &, TargetStateT &)
{
    if(_event.dataLenght() <= s_cmd_length)
    {
        throw ProtocolException(Response(ResponseCode::invalidParameters, "Address is required"),
            "The MAIL FROM request does not contain parameters");
    }
    _event.email().addFromAddress(std::string(&_event.data()[s_cmd_length], _event.dataLenght() - s_cmd_length));
    _protocol.writeResponse(ResponseCode::ok);
    _protocol.listen();
}

class MailFromGuard
{
public:
    template<typename SourceStateT, typename TargetStateT>
    bool operator ()(const EventBase & _event, ProtocolController &, SourceStateT &, TargetStateT &)
    {
        return _event.email().fromAddresses().empty();
    }
}; // class MailFromGuard

class RcptToAction
{
public:
    template<typename SourceStateT, typename TargetStateT>
    void operator ()(const EventBase & _event, ProtocolController & _protocol, SourceStateT &, TargetStateT &);

private:
    static const std::size_t s_cmd_length = sizeof(VERB_RCPT " TO:") - 1;
}; // class RcptToAction

template<typename SourceStateT, typename TargetStateT>
void RcptToAction::operator ()(const EventBase & _event, ProtocolController & _protocol, SourceStateT & _from, TargetStateT & _to)
{
    if(_event.dataLenght() <= s_cmd_length)
    {
        throw ProtocolException(Response(ResponseCode::invalidParameters, "Address is required"),
            "The RCPT TP request does not contain parameters");
    }
    _event.email().addToAddress(std::string(&_event.data()[s_cmd_length], _event.dataLenght() - s_cmd_length));
    _protocol.writeResponse(ResponseCode::ok);
    _protocol.listen();
}

class DataHeaderAction
{
public:
    template<typename SourceStateT, typename TargetStateT>
    void operator ()(const EventBase &, ProtocolController & _protocol, SourceStateT &, TargetStateT &)
    {
        _protocol.setInputMode(InputMode::raw);
        _protocol.writeResponse(ResponseCode::intermediate);
        _protocol.listen();
    }
}; // class DataHeaderAction

class DataAction
{
public:
    template<typename SourceStateT>
    void operator ()(const EventBase & _event, ProtocolController & _protocol, SourceStateT &, DataState & _target_state);

private:
    bool tryWriteTail(const EventBase & _event, DataState & _state);
    bool writeData(const EventBase & _event, DataState & _state);

private:
    static const std::size_t s_end_of_data_mark_length;
}; // class DataAction

const std::size_t DataAction::s_end_of_data_mark_length = sizeof(END_OF_DATA) - 1;

template<typename SourceStateT>
void DataAction::operator ()(const EventBase & _event, ProtocolController & _protocol, SourceStateT &, DataState & _target_state)
{
    if(tryWriteTail(_event, _target_state) || writeData(_event, _target_state))
    {
        _target_state.resetTail();
        _protocol.setInputMode(InputMode::verb);
        _protocol.storeEmail();
        _protocol.writeResponse(ResponseCode::ok);
        _protocol.listen();
    }
    else
    {
        _target_state.resetTail(_event.data(), _event.dataLenght());
        _protocol.listen();
    }
}

bool DataAction::tryWriteTail(const EventBase & _event, DataState & _state)
{
    std::size_t left_length = _state.tailLength();
    if(0 == left_length)
    {
        return false;
    }
    const char * data = _event.data();
    std::size_t data_length = _event.dataLenght();
    std::size_t right_length = std::min(s_end_of_data_mark_length, data_length);
    std::size_t tail_length = left_length + right_length;
    char * tail = new char[tail_length];
    strncpy(tail, _state.tail(), left_length);
    strncpy(&tail[left_length], data, right_length);
    bool result = false;
    const char * end_of_data = std::strstr(tail, END_OF_DATA);
    if(end_of_data)
    {
        end_of_data += s_end_of_data_mark_length;
        const char * begin_data = tail + left_length;
        std::ptrdiff_t data_length = end_of_data - begin_data;
        if(data_length > 0)
            _event.email().data().write(begin_data, data_length);
        result = true;
    }
    delete [] tail;
    return result;
}

bool DataAction::writeData(const EventBase & _event, DataState & _state)
{
    const char * data = _event.data();
    std::size_t data_length = _event.dataLenght();
    boost::iterator_range<const char *> data_range(data, data + data_length);
    boost::iterator_range<const char *> end_of_data_range = boost::find_first(data_range, END_OF_DATA);
    if(end_of_data_range)
    {
        std::size_t length = (end_of_data_range.begin() - data_range.begin()) + s_end_of_data_mark_length;
        _event.email().data().write(data, length);
        return true;
    }
    else
    {
        _event.email().data().write(data, data_length);
        return false;
    }
}

class DataGuard
{
public:
    template<typename SourceStateT, typename TargetStateT>
    bool operator ()(const EventBase &, ProtocolController & _protocol, SourceStateT &, TargetStateT &)
    {
        return _protocol.inputMode() == InputMode::raw;
    }
}; // class DataGuard

class QuitAction
{
public:
    template<typename SourceStateT, typename TargetStateT>
    void operator ()(const EventBase &, ProtocolController & _protocol, SourceStateT &, TargetStateT &)
    {
        _protocol.writeResponse(ResponseCode::closing);
        _protocol.quit();
    }
}; // class QuitAction

class ProtocolImplDef :
    public ProtocolController,
    public boost::msm::front::state_machine_def<ProtocolImplDef>
{
public:
    typedef StartState initial_state;

    struct transition_table : boost::mpl::vector<
        // +------------------+----------------- +---------------- -+-------------------+-----------------+
        // | Start            | Event            | Target           | Action            | Guard           |
        // +------------------+----------------- +---------------- -+-------------------+-----------------+
        Row< StartState       , ReadyEvent       , ReadyState       , ReadyAction        , none           >,
        // +------------------+----------------- +---------------- -+-------------------+-----------------+
        Row< ReadyState       , EhloEvent        , EhloState        , EhloAction        , none            >,
        Row< ReadyState       , QuitEvent        , QuitState        , QuitAction        , none            >,
        // +------------------+----------------- +---------------- -+-------------------+-----------------+
        Row< EhloState        , MailFromEvent    , MailFromState    , MailFromAction    , none            >,
        Row< EhloState        , StartTlsEvent    , StartTlsState    , StartTlsAction    , StartTlsGuard   >,
        Row< EhloState        , QuitEvent        , QuitState        , QuitAction        , none            >,
        // +------------------+----------------- +---------------- -+-------------------+-----------------+
        Row< StartTlsState    , EhloEvent        , EhloState        , EhloAction        , none           >,
        // +------------------+----------------- +---------------- -+-------------------+-----------------+
        Row< MailFromState    , MailFromEvent    , MailFromState    , MailFromAction    , MailFromGuard   >,
        Row< MailFromState    , RcptToEvent      , RcptToState      , RcptToAction      , none            >,
        Row< MailFromState    , QuitEvent        , QuitState        , QuitAction        , none            >,
        // +------------------+----------------- +---------------- -+-------------------+-----------------+
        Row< RcptToState      , RcptToEvent      , RcptToState      , RcptToAction      , none            >,
        Row< RcptToState      , DataHeaderEvent  , DataHeaderState  , DataHeaderAction  , none            >,
        Row< RcptToState      , QuitEvent        , QuitState        , QuitAction        , none            >,
        // +------------------+----------------- +---------------- -+-------------------+-----------------+
        Row< DataHeaderState  , RawDataEvent     , DataState        , DataAction        , none            >,
        // +------------------+----------------- +---------------- -+-------------------+-----------------+
        Row< DataState        , RawDataEvent     , DataState        , DataAction        , DataGuard       >,
        // +------------------+----------------- +---------------- -+-------------------+-----------------+
        Row< DataState        , MailFromEvent    , MailFromState    , MailFromAction    , none            >,
        Row< DataState        , QuitEvent        , QuitState        , QuitAction        , none            >
        // +------------------+----------------- +---------------- -+-------------------+-----------------+
    > { };

public:
    ProtocolImplDef(Repository & _repositry, ProtocolTransport & _transport) :
        ProtocolController(_repositry, _transport)
    {
    }

protected:
    void no_transition(const EventBase & , ProtocolImplDef &, int)
    {
        throw ProtocolException(ResponseCode::badCommandsSequence, "Bad commands sequence");
    }

    void exception_caught(const EventBase & , ProtocolImplDef &, std::exception & _error)
    {
        throw ProtocolException(ResponseCode::badCommandsSequence,
            std::string("State machine's exception has occurred: ") + _error.what());
    }
}; // class ProtocolImplDef

std::ptrdiff_t findEndOfLinePosition(const char * _data, size_t _data_length)
{
    boost::iterator_range<const char *> data_range(_data, _data + _data_length);
    boost::iterator_range<const char *> end_line_range = boost::find_first(data_range, MU_SMTP_ENDLINE);
    return end_line_range ? end_line_range.begin() - data_range.begin() : -1;
}

} // namespace

class Protocol::ProtocolImpl :
    public boost::msm::back::state_machine<ProtocolImplDef, Repository &, ProtocolTransport &>
{
public:
    ProtocolImpl(Repository & _repository, ProtocolTransport & _transport) :
        state_machine(boost::ref(_repository), boost::ref(_transport))
    {
    }

    template<typename EventT>
    void processEvent(const char * _data, std::size_t _data_length) noexcept;
}; // class Protocol::ProtocolImpl

template<typename EventT>
void Protocol::ProtocolImpl::processEvent(const char * _data, std::size_t _data_length) noexcept
{
    static_assert(std::is_base_of<EventBase, EventT>::value,
        "The EventT type must be derived from the EventBase class");
    static_assert(std::is_constructible<EventT, const char *, std::size_t, RawEmail &>::value,
        "The EventT type must have a counstructor compatible with the EventBase's one");
    try
    {
        if(process_event(EventT(_data, _data_length, rawEmail())) == boost::msm::back::HANDLED_GUARD_REJECT)
        {
            writeResponse(ResponseCode::unrecognizedCommand);
            listen();
        }
    }
    catch(const ProtocolException & error)
    {
        LOG_ERROR << "SMTP error: " << error.what();
        writeResponse(error.response());
        listen();
    }
    catch(const std::exception & error)
    {
        LOG_ERROR << "SMTP error: " << error.what();
        writeResponse(ResponseCode::internalError);
        listen();
    }
    catch(...)
    {
        LOG_ERROR << "SMTP unknown error";
        writeResponse(ResponseCode::internalError);
        quit();
    }
}

bool ProtocolTransport::callNextAction()
{
    if(m_next_actions.empty())
    {
        return false;
    }
    Action action = m_next_actions.front();
    m_next_actions.pop();
    if(nullptr == action)
    {
        return callNextAction();
    }
    action();
    return true;
}

Protocol::Protocol(Repository & _repository, ProtocolTransport & _transport) :
    mp_impl(new ProtocolImpl(boost::ref(_repository), boost::ref(_transport))),
    mp_data(nullptr),
    m_data_length(0)
{
}

Protocol::~Protocol()
{
    delete mp_impl;
    free(mp_data);
}

void Protocol::enableStartTls(bool _enable)
{
    if(_enable)
        mp_impl->registerExtenstion(ProtocolExtenstionId::startTls);
    else
        mp_impl->unregisterExtenstion(ProtocolExtenstionId::startTls);
}

void Protocol::start() noexcept
{
    mp_impl->processEvent<ReadyEvent>(nullptr, 0);
}

void Protocol::processInput(const char * _data, size_t _data_length) noexcept
{
    if(nullptr == _data || 0 == _data_length)
    {
        return;
    }
    if(mp_impl->inputMode() == InputMode::raw)
    {
        resetData();
        mp_impl->processEvent<RawDataEvent>(_data, _data_length);
        return;
    }
    std::ptrdiff_t end_of_line_pos = findEndOfLinePosition(_data, _data_length);
    if(end_of_line_pos < 0)
    {
        extendData(_data, _data_length);
        mp_impl->listen();
        return;
    }
    extendData(_data, end_of_line_pos);
    BOOST_SCOPE_EXIT(this_) {
        this_->resetData();
    } BOOST_SCOPE_EXIT_END

    if(strncmp(VERB_EHLO, mp_data, sizeof(VERB_EHLO) - 1) == 0)
    {
        mp_impl->processEvent<EhloEvent>(_data, _data_length);
    }
    else if(strncmp(VERB_MAIL, mp_data, sizeof(VERB_MAIL) - 1) == 0)
    {
        mp_impl->processEvent<MailFromEvent>(_data, _data_length);
    }
    else if(strncmp(VERB_RCPT, mp_data, sizeof(VERB_RCPT) - 1) == 0)
    {
        mp_impl->processEvent<RcptToEvent>(_data, _data_length);
    }
    else if(strncmp(VERB_DATA, mp_data, sizeof(VERB_DATA) - 1) == 0)
    {
        mp_impl->processEvent<DataHeaderEvent>(_data, _data_length);
    }
    else if(strncmp(VERB_STARTTLS, mp_data, sizeof(VERB_STARTTLS) - 1) == 0)
    {
        mp_impl->processEvent<StartTlsEvent>(_data, _data_length);
    }
    else if(strncmp(VERB_QUIT, mp_data, sizeof(VERB_QUIT) - 1) == 0)
    {
        mp_impl->processEvent<QuitEvent>(_data, _data_length);
    }
    else
    {
        mp_impl->writeResponse(ResponseCode::unrecognizedCommand);
        mp_impl->listen();
    }
}

void Protocol::resetData() noexcept
{
    free(mp_data);
    m_data_length = 0;
    mp_data = nullptr;
}

void Protocol::extendData(const char * _data, size_t _data_length) noexcept
{
    size_t new_length = m_data_length + _data_length;
    mp_data = static_cast<char *>(nullptr == mp_data ? malloc(new_length) : realloc(mp_data, new_length));
    memcpy(&mp_data[m_data_length], _data, _data_length);
    m_data_length = new_length;
}
