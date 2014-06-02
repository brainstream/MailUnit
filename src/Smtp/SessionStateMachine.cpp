#include <cstring>
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/back/state_machine.hpp>
#include <boost/mpl/vector.hpp>
#include "SessionStateMachine.h"
#include "Message.h"

#define SMTP_CMDLEN 4
#define SMTP_CMD_EHLO "EHLO"
#define SMTP_CMD_MAIL "MAIL"
#define SMTP_CMD_RCPT "RCPT"
#define SMTP_CMD_DATA "DATA"
#define SMTP_CMD_QUIT "QUIT"

using namespace StubMTP::Smtp;

namespace {


class Event
{
public:
    Event(const std::string & _data) :
        m_data(_data)
    {
    }

    const std::string & data() const
    {
        return m_data;
    }

private:
    std::string m_data;
}; // class Event

#define EVENT(name)                       \
    class name : public Event             \
    {                                     \
    public:                               \
        name(const std::string & _data) : \
            Event(_data)                  \
        {                                 \
        }                                 \
    };

EVENT(EhloEvent)
EVENT(MailFromEvent)
EVENT(RcptToEvent)
EVENT(DataEvent)
EVENT(QuitEvent)

class StateBase
{
public:
    virtual ~StateBase()
    {
    }

    template <typename EventT, typename FsmT>
    void on_entry(const EventT & _event, FsmT &)
    {
        std::cout << "Entering state: " << name() << std::endl;
        m_data = _event.data();
    }

    template <typename EventT, typename FsmT>
    void on_exit(const EventT &, FsmT &)
    {
        std::cout << "Leaving state: " << name() << std::endl;
        m_data.empty();
    }

    // TODO:
    //const Message & message() const
    //{
    //    return *mp_message;
    //}

    virtual bool finished() const
    {
        return false;
    }

    virtual const std::string & name() const = 0;
    virtual bool getResponse(SessionStateMachine::Response & _response) const = 0;

protected:
    const std::string & data() const
    {
        return m_data;
    }

private:
    // TODO: Message * mp_message;
    std::string m_data;
}; // class StateBase

class State : public boost::msm::front::state<StateBase>
{
public:
    explicit State(const std::string & _name) :
        m_name(_name)
    {
    }

    const std::string & name() const override
    {
        return m_name;
    }

private:
    std::string m_name;
}; // class State

class BeginState : public State
{
public:
    BeginState() : State("<BEGIN>") { }

    bool getResponse(SessionStateMachine::Response & _response) const override
    {
        _response.ready = true;
        _response.message = "220 [127.0.0.1] Stub Mail Transfer Service Ready\r\n";
        return true;
    }
}; // class BeginState

class EhloState : public State
{
public:
    EhloState() : State(SMTP_CMD_EHLO) { }

    bool getResponse(SessionStateMachine::Response & _response) const override
    {
        _response.ready = true;
        _response.message = "250 OK\r\n";
        return true;
    }
}; // class EhloState

class MailFromState : public State
{
public:
    MailFromState() : State(SMTP_CMD_RCPT) { }

    bool getResponse(SessionStateMachine::Response & _response) const override
    {
        // TODO: parse address!
        _response.ready = true;
        _response.message = "250 OK\r\n";
        return true;
    }
}; // class MailState

class RcptToState : public State
{
public:
    RcptToState() : State(SMTP_CMD_RCPT) { }

    bool getResponse(SessionStateMachine::Response & _response) const override
    {
        _response.ready = true;
        _response.message = "250 OK\r\n";
        return true;
    }
}; // class RcptToState

class DataState : public State
{
public:
    DataState() : State(SMTP_CMD_DATA) { }

    bool getResponse(SessionStateMachine::Response & _response) const override
    {
        if(data() == "\r\n.\r\n")
        {
            _response.message = "250 OK\r\n";
        }
        else
        {
            // TODO: append data to a buffer!
            _response.message = "354 Intermediate\r\n";
        }
        _response.ready = true;
        return true;
    }
}; // class RcptState

class QuitState : public State
{
public:
    QuitState() : State(SMTP_CMD_QUIT) { }

    bool getResponse(SessionStateMachine::Response & _response) const override
    {
        _response.ready = false;
        return false;
    }

    bool finished() const override
    {
        return true;
    }
}; // class RcptQuit


class StateMachineDef :
    public boost::msm::front::state_machine_def<StateMachineDef>
{
    typedef StateMachineDef smd;


public:
    typedef BeginState initial_state;
    typedef StateBase BaseAllStates;

    // TODO: delete?
    template <typename EventT, typename FsmT>
    void on_entry(const EventT & , FsmT &)
    {
        std::cout << "Entering: StateMachine" << std::endl;
    }

    // TODO: delete?
    template <typename EventT, typename FsmT>
    void on_exit(const Event &, FsmT &)
    {
        std::cout << "Leaving: StateMachine" << std::endl;
    }

    void ehlo(const EhloEvent & _event)
    {
        // TODO: move logic from state?!
        std::cout << "Handling EHLO...\n";
    }

    void mailFrom(const MailFromEvent & _event)
    {
        // TODO: move logic from state?!
        std::cout << "Handling MAIL FROM...\n";
    }

    void rcptTo(const RcptToEvent & _event)
    {
        // TODO: move logic from state?!
        std::cout << "Handling RCPT TO...\n";
    }

    void data(const DataEvent & _event)
    {
        // TODO: move logic from state?!
        std::cout << "Handling DATA...\n";
    }

    void quit(const QuitEvent & _event)
    {
        // TODO: move logic from state?!
        std::cout << "Handling QUIT...\n";
    }

public: // TODO: protected?
    struct transition_table : boost::mpl::vector<
        //     Start          Event          Next           Action         Guard
        a_row< BeginState,    EhloEvent,     EhloState,     &smd::ehlo     /*, None */ >,
        a_row< EhloState,     MailFromEvent, MailFromState, &smd::mailFrom /*, None */ >,
        a_row< MailFromState, MailFromEvent, MailFromState, &smd::mailFrom /*, None */ >,
        a_row< MailFromState, RcptToEvent,   RcptToState,   &smd::rcptTo   /*, None */ >,
        a_row< RcptToState,   RcptToEvent,   RcptToState,   &smd::rcptTo   /*, None */ >,
        a_row< RcptToState,   DataEvent,     DataState,     &smd::data     /*, None */ >,
        a_row< RcptToState,   QuitEvent,     QuitState,     &smd::quit     /*, None */ >
    > { };
}; // class SessionStateMachine::StateMachine


typedef boost::msm::back::state_machine<StateMachineDef> StateMachine;

} // namespace


struct SessionStateMachine::Private
{
    StateMachine state_machine;
}; // struct SessionStateMachine::Private


SessionStateMachine::SessionStateMachine() :
    mp_private(new Private())
{
}

SessionStateMachine::~SessionStateMachine()
{
    delete mp_private;
}

SessionStateMachine::Response SessionStateMachine::start()
{
    // TODO: clear states
    mp_private->state_machine.start();
    return getResponse();
}

SessionStateMachine::Response SessionStateMachine::getResponse()
{
    int current_state_id = mp_private->state_machine.current_state()[0];
    const StateBase * state = mp_private->state_machine.get_state_by_id(current_state_id);
    Response response;
    state->getResponse(response);
    return response;
}

SessionStateMachine::Response SessionStateMachine::pushRequest(const std::string & _string)
{

    if(_string.length() < SMTP_CMDLEN)
    {
        // TODO: set _string to mp_machine->current_state()[0];
    }
    else if(_string.compare(0, SMTP_CMDLEN, SMTP_CMD_EHLO) == 0)
    {
        mp_private->state_machine.process_event(EhloEvent(_string));
    }
    else if(_string.compare(0, SMTP_CMDLEN, SMTP_CMD_MAIL) == 0)
    {
        mp_private->state_machine.process_event(MailFromEvent(_string));
    }
    else if(_string.compare(0, SMTP_CMDLEN, SMTP_CMD_RCPT) == 0)
    {
        mp_private->state_machine.process_event(RcptToEvent(_string));
    }
    else if(_string.compare(0, SMTP_CMDLEN, SMTP_CMD_DATA) == 0)
    {
        mp_private->state_machine.process_event(DataEvent(_string));
    }
    else if(_string.compare(0, SMTP_CMDLEN, SMTP_CMD_QUIT) == 0)
    {
        mp_private->state_machine.process_event(QuitEvent(_string));
    }
    else
    {
        // TODO: set _string to mp_machine->current_state()[0];
    }
    return getResponse();
}
