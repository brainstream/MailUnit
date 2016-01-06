/***********************************************************************************************
 *                                                                                             *
 * This file is part of the MailUnit Library.                                                  *
 *                                                                                             *
 * MailUnit Library is free software: you can redistribute it and/or modify it under the terms *
 * of the GNU Lesser General Public License as published by the Free Software Foundation,      *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * MailUnit Library is distributed in the hope that it will be useful, but WITHOUT ANY         *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  *
 * PURPOSE. See the GNU Lesser General Public License for more details.                        *
 *                                                                                             *
 * You should have received a copy of the GNU License General Public License along with        *
 * MailUnit Library. If not, see <http://www.gnu.org/licenses>.                                *
 *                                                                                             *
 ***********************************************************************************************/

#include <thread>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_array.hpp>
#include <boost/asio.hpp>
#include <LibMailUnit/Mqp/Client.h>

using namespace LibMailUnit::Mqp;
namespace asio = boost::asio;

namespace {

class SharedIOService final
{
public:
    SharedIOService()
    {
        if(0 == s_counter++) initService();
    }

    SharedIOService(const SharedIOService &)
    {
        if(0 == s_counter++) initService();
    }

    SharedIOService(SharedIOService &&) = delete;

    ~SharedIOService()
    {
        if(0 == --s_counter) deinitService();
    }

    SharedIOService & operator = (const SharedIOService & _rhs)
    {
        if(&_rhs != this && 0 == s_counter++) initService();
        return *this;
    }

    SharedIOService & operator = (SharedIOService &&) = delete;

    asio::io_service & service() const
    {
        return *s_service;
    }

private:
    void initService();
    void deinitService();

private:
    static size_t s_counter;
    static const size_t s_thread_count = 4; // TODO: config or std::thread::hardware_concurrency()
    static std::vector<std::thread *> s_threads;
    static std::shared_ptr<asio::io_service> s_service;
    static std::shared_ptr<asio::io_service::work> s_dummy_work;
}; // class Client::SharedIOService

size_t SharedIOService::s_counter = 0;
std::shared_ptr<asio::io_service> SharedIOService::s_service;
std::vector<std::thread *> SharedIOService::s_threads;
std::shared_ptr<asio::io_service::work> SharedIOService::s_dummy_work;

void SharedIOService::initService()
{
    s_service.reset(new asio::io_service());
    s_threads.reserve(s_thread_count);
    s_dummy_work.reset(new asio::io_service::work(*s_service));
    for(size_t i = 0; i < s_thread_count; ++i)
    {
        std::thread * thread = new std::thread([]() {
            s_service->run();
        });
        s_threads.push_back(thread);
        thread->detach();
    }
}

void SharedIOService::deinitService()
{
    s_dummy_work.reset();
    for(size_t i = 0; i < s_thread_count; ++i)
    {
        delete s_threads[i];
    }
    s_threads.clear();
    s_service.reset();
}

} // namespace

class Client::Session final : public std::enable_shared_from_this<Client::Session>
{
public:
    explicit Session(const Command & _command);
    ~Session();
    void run(const std::string _host, unsigned short _port);
    void abort();

private:
    void connect(const asio::ip::tcp::endpoint & _endpoint);
    void writeQuery();
    void readResponseHeader();
    void readMessages();
    void readMessageHeader();
    void readMessageBody();
    void writeQuit();
    void raiseError(const boost::system::error_code & _error);

private:
    SharedIOService m_shared_io_service;
    asio::ip::tcp::socket * mp_socket;
    asio::ip::tcp::resolver m_name_resolver;
    asio::streambuf m_streambuff;
    const Command * mp_command;
    ResponseHeader * mp_response_header; // TODO: must not be a pointer (?)
    Message * mp_current_message;
    size_t m_total_message_count;
    size_t m_current_message_number;
}; // class Client::Session


struct Client::CommandInfo
{
    std::shared_ptr<Client::Session> session;
}; // struct Client::CommandInfo


Client::Session::Session(const Command & _command) :
    mp_socket(nullptr),
    m_name_resolver(m_shared_io_service.service()),
    mp_command(&_command),
    mp_response_header(new ResponseHeader),
    mp_current_message(nullptr),
    m_total_message_count(0),
    m_current_message_number(0)
{
    memset(mp_response_header, 0, sizeof(ResponseHeader));
}

Client::Session::~Session()
{
    if(mp_socket->is_open())
        mp_socket->close();
    delete mp_socket;
    delete mp_response_header;
    delete mp_current_message;
}

void Client::Session::run(const std::string _host, unsigned short _port)
{
    std::shared_ptr<Client::Session> self = shared_from_this();
    auto query = asio::ip::tcp::resolver::query(_host, boost::lexical_cast<std::string>(_port));
    m_name_resolver.async_resolve(query, [self](const boost::system::error_code& error,
        asio::ip::tcp::resolver::iterator iterator)
    {
        if(error)
        {
            self->raiseError(error);
            return;
        }
        self->connect(*iterator);
    });
}

void Client::Session::connect(const asio::ip::tcp::endpoint & _endpoint)
{
    mp_socket = new asio::ip::tcp::socket(m_shared_io_service.service());
    std::shared_ptr<Client::Session> self = shared_from_this();
    mp_socket->async_connect(_endpoint, [self](const boost::system::error_code & error) {
        if(error)
        {
            self->raiseError(error);
            return;
        }
        self->writeQuery();
    });
}

void Client::Session::writeQuery()
{
    std::string query = boost::trim_copy(mp_command->query());
    if(!boost::ends_with(query, ";"))
        query += ';';
    std::shared_ptr<Client::Session> self = shared_from_this();
    mp_socket->async_write_some(asio::buffer(query), [self](boost::system::error_code error, size_t) {
        if(error)
        {
            self->raiseError(error);
            return;
        }
        self->readResponseHeader();
    });
}

void Client::Session::readResponseHeader()
{
    static const char mqp_response_header_delemiter[] = "\r\n\r\n";
    std::shared_ptr<Session> self = shared_from_this();
    asio::streambuf * response = new asio::streambuf();
    mp_response_header->response_type = ResponseType::error;
    asio::async_read_until(*mp_socket, *response, mqp_response_header_delemiter,
        [self, response](const boost::system::error_code & error, size_t) {
            static const char hdr_status[]  = "STATUS: ";
            static const char hdr_matched[] = "MATCHED: ";
            static const char hdr_deleted[] = "DELETED: ";
            if(error)
            {
                self->raiseError(error);
                return;
            }
            std::istream response_stream(response);
            std::string line;
            while(std::getline(response_stream, line))
            {
                boost::trim(line);
                if(line.empty()) {
                    break;
                }
                if(boost::starts_with(line, hdr_status))
                {
                    self->mp_response_header->status_code =
                            boost::lexical_cast<unsigned int>(line.substr(sizeof(hdr_status) - 1));
                }
                else if(boost::starts_with(line, hdr_matched))
                {
                    self->mp_response_header->affected_count =
                        boost::lexical_cast<unsigned int>(line.substr(sizeof(hdr_matched) - 1));
                    self->mp_response_header->response_type = ResponseType::matched;
                }
                else if(boost::starts_with(line, hdr_deleted))
                {
                    self->mp_response_header->affected_count =
                        boost::lexical_cast<unsigned int>(line.substr(sizeof(hdr_deleted) - 1));
                    self->mp_response_header->response_type = ResponseType::deleted;
                }
            }
            delete response;
            self->mp_command->callObservers([self](CommandExecutionObserver & observer) {
                observer.onResponseHeaderReceived(*self->mp_command, *self->mp_response_header);
            });
            if(ResponseType::matched == self->mp_response_header->response_type)
            {
                self->m_total_message_count = self->mp_response_header->affected_count;
                self->m_current_message_number = 0;
                self->readMessages();
            }
            else
            {
                self->writeQuit();
            }
        });
}

void Client::Session::readMessages()
{
    static const char mqp_header_body_delemiter[] = "\r\n\r\n";
    if(m_current_message_number == m_total_message_count)
    {
        writeQuit();
        return;
    }
    ++m_current_message_number;
    std::shared_ptr<Session> self = shared_from_this();
    asio::async_read_until(*mp_socket, m_streambuff, mqp_header_body_delemiter, [self](const boost::system::error_code & error, size_t) {
        if(error)
        {
            self->raiseError(error);
            return;
        }
        self->readMessageHeader();
    });
}

void Client::Session::readMessageHeader()
{
    static const char hdr_size[]    = "SIZE: ";
    static const char hdr_id[]      = "ID: ";
    static const char hdr_subject[] = "SUBJECT: ";
    static const char hdr_from[]    = "FROM: ";
    static const char hdr_to[]      = "TO: ";
    static const char hdr_cc[]      = "CC: ";
    static const char hdr_bcc[]     = "BCC: ";
    mp_current_message = new Message { };
    mp_current_message->number = m_current_message_number;
    std::istream header_stream(&m_streambuff);
    std::string line;
    for(int i = 0; std::getline(header_stream, line); ++i)
    {
        boost::trim(line);
        if(line.empty() && i)
        {
            break;
        }
        else if(boost::starts_with(line, hdr_size))
        {
            mp_current_message->length = boost::lexical_cast<size_t>(line.substr(sizeof(hdr_size) - 1));
        }
        else if(boost::starts_with(line, hdr_id))
        {
            mp_current_message->id = boost::lexical_cast<size_t>(line.substr(sizeof(hdr_id) - 1));
        }
        else if(boost::starts_with(line, hdr_from))
        {
            mp_current_message->from.push_back(line.substr(sizeof(hdr_from) - 1));
        }
        else if(boost::starts_with(line, hdr_to))
        {
            mp_current_message->to.push_back(line.substr(sizeof(hdr_to) - 1));
        }
        else if(boost::starts_with(line, hdr_cc))
        {
            mp_current_message->cc.push_back(line.substr(sizeof(hdr_cc) - 1));
        }
        else if(boost::starts_with(line, hdr_bcc))
        {
            mp_current_message->bcc.push_back(line.substr(sizeof(hdr_bcc) - 1));
        }
        else if(boost::starts_with(line, hdr_subject))
        {
            mp_current_message->subject = line.substr(sizeof(hdr_subject) - 1);
        }
    }
    if(0 == mp_current_message->length)
    {
        // TODO: error
        return;
    }
    mp_current_message->body = new char[mp_current_message->length + 1];
    memset(mp_current_message->body, 0, mp_current_message->length + 1);
    auto self = shared_from_this();
    asio::async_read(*mp_socket, m_streambuff, [self](const boost::system::error_code & error, size_t) {
        if(error)
        {
            self->raiseError(error);
            return size_t(0);
        }
        std::ptrdiff_t remining_bytes = self->mp_current_message->length - asio::buffer_size(self->m_streambuff.data());
        return size_t(remining_bytes > 0 ? remining_bytes : 0);
    }, [self](const boost::system::error_code & error, size_t) {
        if(error)
        {
            self->raiseError(error);
            return;
        }
        self->readMessageBody();
    });
}

void Client::Session::readMessageBody()
{
    std::istream body_stream(&m_streambuff);
    body_stream.read(mp_current_message->body, mp_current_message->length);
    mp_command->callObservers([this](CommandExecutionObserver & observer) {
        observer.onMessageReceived(*mp_command, *mp_current_message);
    });
    readMessages();
    delete [] mp_current_message->body;
    delete mp_current_message;
    mp_current_message = nullptr;
}

void Client::Session::writeQuit()
{
    std::shared_ptr<Client::Session> self = shared_from_this();
    mp_socket->async_write_some(asio::buffer("quit;"), [self](boost::system::error_code error, size_t) {
        if(error)
        {
            self->raiseError(error);
            return;
        }
        self->mp_socket->close();
        self->mp_command->callObservers([self](CommandExecutionObserver & observer) {
            observer.onFinished(*self->mp_command);
        });
    });
}


void Client::Session::abort()
{
    // TODO: if option allows -> send "abort" | set the abort flag and handle it in the readMessages etc
}

void Client::Session::raiseError(const boost::system::error_code & _error)
{
    mp_command->callObservers([this, &_error](CommandExecutionObserver & observer) {
        observer.onError(*mp_command, _error);
    });
}

Client::Client(const std::string & _host, unsigned short _port) :
    m_host(_host),
    m_port(_port)
{
}

Client::~Client()
{
    for(const std::pair<const Command *, CommandInfo *> & pair : m_commands)
    {
        std::shared_ptr<Session> & session = pair.second->session;
        session.reset();
    }
}

void Client::executeCommand(const Command & _command)
{
    Session * session = new Session(_command);
    CommandInfo * command_info = new CommandInfo;
    command_info->session.reset(session);
    m_commands[&_command] = command_info;
    session->run(m_host, m_port);
}

void Client::abortCommand(const Command & _command)
{
    auto iterator = m_commands.find(&_command);
    if(m_commands.end() != iterator)
    {
        std::shared_ptr<Session> session;
        iterator->second->session.swap(session);
        if(nullptr == session)
            return;
        m_commands.erase(iterator);
        session->abort();
    }
}
