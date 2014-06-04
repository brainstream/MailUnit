#include <iostream>
#include "SessionProvider.h"
#include "StateMachine/StateMachine.h"

// TODO: move to a separate file!
#define SMTP_CMDLEN   4
#define SMTP_CMD_EHLO "EHLO"
#define SMTP_CMD_MAIL "MAIL"
#define SMTP_CMD_RCPT "RCPT"
#define SMTP_CMD_DATA "DATA"
#define SMTP_CMD_QUIT "QUIT"

using namespace StubMTP::Smtp;

namespace {

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(boost::asio::ip::tcp::socket _socket, std::shared_ptr<SessionProvider> _provider);
    ~Session();
    void start();

private:
    void performNextAction();
    void write(const std::string & _message);
    void read();
    void processInput(const std::string & _input);

private:
    boost::asio::ip::tcp::socket m_socket;
    std::shared_ptr<SessionProvider> m_provider_ptr;
    boost::asio::streambuf m_buffer; // TODO: move to the using point!
    StateMachine * mp_state_machine;
}; // class Session

} // namespace



SessionProvider::SessionProvider(boost::asio::io_service & _io_service) :
    mr_io_service(_io_service)
{
}

SessionProvider::~SessionProvider()
{
}

void SessionProvider::startNewSession(boost::asio::ip::tcp::socket _socket)
{
    std::make_shared<Session>(std::move(_socket), shared_from_this())->start();
}

Session::Session(boost::asio::ip::tcp::socket _socket, std::shared_ptr<SessionProvider> _provider) :
    m_socket(std::move(_socket)),
    m_provider_ptr(_provider),
    mp_state_machine(new StateMachine())
{
}

Session::~Session()
{
    delete mp_state_machine;
}

void Session::start()
{
    performNextAction();
}

void Session::performNextAction()
{
    int state_id = mp_state_machine->current_state()[0];
    StateBase * state = mp_state_machine->get_state_by_id(state_id);
    ResponseCode response;
    if(state->response(&response))
        write(translateResponseCode(response));
    else
        read();
}

void Session::write(const std::string & _message)
{
    auto self(shared_from_this());
    m_socket.async_send(boost::asio::buffer(_message),
        [this, self](const boost::system::error_code & ec, std::size_t length)
        {
            read();
        });
}

void Session::read()
{
    static std::string until("\r\n");
    m_buffer.prepare(1024);

    auto self(shared_from_this());
    boost::asio::async_read_until(m_socket, m_buffer, '\n',
        [this, self](const boost::system::error_code & ec, std::size_t length)
        {
            if(ec) return;
            m_buffer.commit(length);
            std::istream is(&m_buffer);
            size_t size = m_buffer.size();
            char * str_data = new char[size + 1];
            memset(str_data, 0, size + 1);
            is.readsome(str_data, size);
            std::string str(str_data);
            delete [] str_data;
            processInput(str);
            performNextAction();
        });
}

void Session::processInput(const std::string & _input)
{
    std::cout << _input;
    int state_id = mp_state_machine->current_state()[0];
    StateBase * state = mp_state_machine->get_state_by_id(state_id);
    if(!state->isInutProcessingCompleted())
    {
        state->processInput(_input);
        return;
    }
    if(_input.length() < SMTP_CMDLEN)
    {
        // TODO: set _input to mp_machine->current_state()[0];
    }
    else if(_input.compare(0, SMTP_CMDLEN, SMTP_CMD_EHLO) == 0)
    {
        mp_state_machine->process_event(EhloEvent(_input));
    }
    else if(_input.compare(0, SMTP_CMDLEN, SMTP_CMD_MAIL) == 0)
    {
        mp_state_machine->process_event(MailFromEvent(_input));
    }
    else if(_input.compare(0, SMTP_CMDLEN, SMTP_CMD_RCPT) == 0)
    {
        mp_state_machine->process_event(RcptToEvent(_input));
    }
    else if(_input.compare(0, SMTP_CMDLEN, SMTP_CMD_DATA) == 0)
    {
        mp_state_machine->process_event(DataEvent(_input));
    }
    else if(_input.compare(0, SMTP_CMDLEN, SMTP_CMD_QUIT) == 0)
    {
        mp_state_machine->process_event(QuitEvent(_input));
    }
    else
    {
        // TODO: set _input to mp_machine->current_state()[0];
    }
    state_id = mp_state_machine->current_state()[0];
    state = mp_state_machine->get_state_by_id(state_id);
    state->processInput(_input);
}

