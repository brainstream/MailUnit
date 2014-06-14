/***********************************************************************************************
 *                                                                                             *
 * This file is part of StubMTP.                                                               *
 *                                                                                             *
 * StubMTP is free software: you can redistribute it and/or modify it under the terms of       *
 * the GNU General Public License as published by the Free Software Foundation,                *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * StubMTP is distributed in the hope that it will be useful,  but WITHOUT ANY WARRANTY;       *
 * without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  *
 * See the GNU General Public License for more details.                                        *
 *                                                                                             *
 * You should have received a copy of the GNU General Public License along with StubMTP.       *
 * If not, see <http://www.gnu.org/licenses/>.                                                 *
 *                                                                                             *
 ***********************************************************************************************/

#include <iostream>
#include <StubMTP/Smtp/SessionProvider.h>
#include <StubMTP/Smtp/ProtocolDef.h>
#include <StubMTP/Smtp/StateMachine/StateMachine.h>

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
    static const size_t s_buffer_size = 1024;
    boost::asio::ip::tcp::socket m_socket;
    std::shared_ptr<SessionProvider> m_provider_ptr;
    StateMachine * mp_state_machine;
    Message * mp_message;
    char * m_buffer;
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
    mp_state_machine(new StateMachine()),
    mp_message(new Message()),
    m_buffer(new char[s_buffer_size])
{
}

Session::~Session()
{
    delete mp_state_machine;
    delete mp_message;
    delete [] m_buffer;
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
    if(state->isProtocolProcessingCompleted())
        m_provider_ptr->onMessageRecieved(*mp_message);
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
    auto self(shared_from_this());
    m_socket.async_receive(boost::asio::buffer(m_buffer, s_buffer_size - 1),
        [this, self](const boost::system::error_code & ec, std::size_t length)
        {
            if(ec) return; // TODO: log
            m_buffer[length] = '\0';
            processInput(m_buffer);
            performNextAction();
        });
}

void Session::processInput(const std::string & _input)
{
    int state_id = mp_state_machine->current_state()[0];
    StateBase * state = mp_state_machine->get_state_by_id(state_id);
    if(!state->isInutProcessingCompleted())
    {
        state->processInput(_input, *mp_message);
        return;
    }
    else if(_input.compare(0, SMTP_CMDLEN, SMTP_CMD_EHLO) == 0)
    {
        mp_state_machine->process_event(EhloEvent(_input, *mp_message));
    }
    else if(_input.compare(0, SMTP_CMDLEN, SMTP_CMD_MAIL) == 0)
    {
        mp_state_machine->process_event(MailFromEvent(_input, *mp_message));
    }
    else if(_input.compare(0, SMTP_CMDLEN, SMTP_CMD_RCPT) == 0)
    {
        mp_state_machine->process_event(RcptToEvent(_input, *mp_message));
    }
    else if(_input.compare(0, SMTP_CMDLEN, SMTP_CMD_DATA) == 0)
    {
        mp_state_machine->process_event(DataEvent(_input, *mp_message));
    }
    else if(_input.compare(0, SMTP_CMDLEN, SMTP_CMD_QUIT) == 0)
    {
        mp_state_machine->process_event(QuitEvent(_input, *mp_message));
    }
    else
    {
        // TODO: error!
        return;
    }
}

