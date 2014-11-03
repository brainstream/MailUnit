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

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <MailUnit/Def.h>
#include <MailUnit/Smtp/ServerRequestHandler.h>
#include <MailUnit/Smtp/ProtocolDef.h>
#include <MailUnit/Smtp/StateMachine/StateMachine.h>

using namespace MailUnit;
using namespace MailUnit::Smtp;
using namespace MailUnit::Storage;

namespace {

class Session final : public std::enable_shared_from_this<Session>, private boost::noncopyable
{
public:
    inline Session(boost::asio::ip::tcp::socket _socket, std::shared_ptr<Repository> _repository);
    ~Session();
    inline void start();

private:
    void performNextAction();
    void write(const std::string & _message);
    void read();
    void processInput(const std::string & _input);
    void saveEmail();

private:
    static const size_t s_buffer_size = 1024;
    boost::asio::ip::tcp::socket m_socket;
    StateMachine * mp_state_machine;
    std::shared_ptr<Repository> m_repository_ptr;
    std::unique_ptr<RawEmail> m_raw_email_ptr;
    char * mp_buffer;
}; // class Session

} // namespace

ServerRequestHandler::ServerRequestHandler(std::shared_ptr<Storage::Repository> _repository) :
    m_repository_ptr(_repository)
{
}

void ServerRequestHandler::handleConnection(boost::asio::ip::tcp::socket _socket)
{
    logger->info("New connection accepted by the SMTP server");
    std::make_shared<Session>(std::move(_socket), m_repository_ptr)->start();
}

bool ServerRequestHandler::handleError(const boost::system::error_code & _err_code)
{
    logger->error(std::string("The SMTP server has stopped due an error: ") + _err_code.message());
    return false;
}

Session::Session(boost::asio::ip::tcp::socket _socket, std::shared_ptr<Repository> _repository) :
    m_socket(std::move(_socket)),
    mp_state_machine(new StateMachine()),
    m_repository_ptr(_repository),
    m_raw_email_ptr(m_repository_ptr->createRawEmail()),
    mp_buffer(new char[s_buffer_size])
{
}

Session::~Session()
{
    delete mp_state_machine;
    delete [] mp_buffer;
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
        saveEmail();
    if(state->response(&response))
        write(translateResponseCode(response));
    else
        read();
}

void Session::saveEmail()
{
    try
    {
        logger->info("Message received"); // TODO: more details
        m_repository_ptr->storeEmail(*m_raw_email_ptr);
    }
    catch(const Storage::StorageException & error)
    {
        logger->error("An error occurred during an attempt to "
            "save an e-mail into the database", error);
    }
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
    m_socket.async_receive(boost::asio::buffer(mp_buffer, s_buffer_size - 1),
        [this, self](const boost::system::error_code & ec, std::size_t length)
        {
            if(ec) return; // TODO: log
            mp_buffer[length] = '\0';
            processInput(mp_buffer);
            performNextAction();
        });
}

void Session::processInput(const std::string & _input)
{
    int state_id = mp_state_machine->current_state()[0];
    StateBase * state = mp_state_machine->get_state_by_id(state_id);
    if(!state->isInputProcessingCompleted())
    {
        state->processInput(_input, *m_raw_email_ptr);
        return;
    }
    else if(_input.compare(0, SMTP_CMDLEN, SMTP_CMD_EHLO) == 0)
    {
        mp_state_machine->process_event(EhloEvent(_input, *m_raw_email_ptr));
    }
    else if(_input.compare(0, SMTP_CMDLEN, SMTP_CMD_MAIL) == 0)
    {
        mp_state_machine->process_event(MailFromEvent(_input, *m_raw_email_ptr));
    }
    else if(_input.compare(0, SMTP_CMDLEN, SMTP_CMD_RCPT) == 0)
    {
        mp_state_machine->process_event(RcptToEvent(_input, *m_raw_email_ptr));
    }
    else if(_input.compare(0, SMTP_CMDLEN, SMTP_CMD_DATA) == 0)
    {
        mp_state_machine->process_event(DataEvent(_input, *m_raw_email_ptr));
    }
    else if(_input.compare(0, SMTP_CMDLEN, SMTP_CMD_QUIT) == 0)
    {
        mp_state_machine->process_event(QuitEvent(_input, *m_raw_email_ptr));
    }
    else
    {
        // TODO: error!
        return;
    }
}
