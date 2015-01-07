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
#include <MailUnit/Logger.h>
#include <MailUnit/Smtp/ServerRequestHandler.h>
#include <MailUnit/Smtp/Protocol.h>

using namespace MailUnit;
using namespace MailUnit::Smtp;
using namespace MailUnit::Storage;

typedef typename boost::asio::ip::tcp::socket TcpSocket;

namespace {

class Session final :
    public std::enable_shared_from_this<Session>,
    public ProtocolTransport,
    private boost::noncopyable
{
public:
    inline Session(TcpSocket _socket, std::shared_ptr<Repository> _repository);
    ~Session() override;
    inline void start();
    void readRequest() override;
    void writeRequest(const std::string & _data) override;
    void switchToTlsRequest() override;
    void exitRequest() override;

private:
    void performNextAction();
    void write(const std::string & _message);
    void read();
    void processInput(const std::string & _input);
    void saveEmail();

private:
    static const size_t s_buffer_size = 1024;
    TcpSocket m_socket;
    std::shared_ptr<Repository> m_repository_ptr;
    char * mp_buffer;
    Protocol * mp_protocol;
}; // class Session

} // namespace

ServerRequestHandler::ServerRequestHandler(std::shared_ptr<Storage::Repository> _repository) :
    m_repository_ptr(_repository)
{
}

void ServerRequestHandler::handleConnection(TcpSocket _socket)
{
    LOG_INFO << "New connection accepted by the SMTP server";
    std::make_shared<Session>(std::move(_socket), m_repository_ptr)->start();
}

bool ServerRequestHandler::handleError(const boost::system::error_code & _err_code)
{
    LOG_ERROR << "The SMTP server has stopped due an error: " << _err_code.message();
    return false;
}

Session::Session(TcpSocket _socket, std::shared_ptr<Repository> _repository) :
    m_socket(std::move(_socket)),
    m_repository_ptr(_repository),
    mp_buffer(new char[s_buffer_size])
{
    mp_protocol = new Protocol(*m_repository_ptr, *this);
}

Session::~Session()
{
    delete [] mp_buffer;
    delete mp_protocol;
}

void Session::start()
{
    mp_protocol->processInput(nullptr);
}

void Session::readRequest()
{
    read();
}

void Session::writeRequest(const std::string & _data)
{
    write(_data);
}

void Session::switchToTlsRequest()
{
}

void Session::exitRequest()
{
    // Just do nothing
}

void Session::write(const std::string & _message)
{
    auto self(shared_from_this());
    m_socket.async_send(boost::asio::buffer(_message),
        [this, self](const boost::system::error_code & ec, std::size_t length)
        {
            afterWritingAction();
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
            mp_protocol->processInput(mp_buffer);
        });
}
