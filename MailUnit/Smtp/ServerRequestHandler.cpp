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

#include <sstream>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/asio/ssl.hpp>
#include <MailUnit/Logger.h>
#include <MailUnit/Server/Tcp/TcpSession.h>
#include <MailUnit/Smtp/Protocol.h>
#include <MailUnit/Smtp/ServerRequestHandler.h>

using namespace MailUnit;
using namespace MailUnit::Server;
using namespace MailUnit::Smtp;
using namespace MailUnit::Storage;

namespace {

class SmtpSession final :
    public std::enable_shared_from_this<SmtpSession>,
    public TcpSession,
    public ProtocolTransport
{
public:
    inline SmtpSession(TcpSocket _socket, std::shared_ptr<Repository> _repository);
    ~SmtpSession() override;
    void start() override;
    void requestForRead() override;
    void requestForWrite(const Response & _response) override;
    void requestForSwitchToTls() override;
    void requestForExit() override;

private:
    static const size_t s_buffer_size = 1024;
    std::shared_ptr<Repository> m_repository_ptr;
    char * mp_buffer;
    Protocol * mp_protocol;
}; // class SmtpSession

} // namespace

ServerRequestHandler::ServerRequestHandler(std::shared_ptr<Storage::Repository> _repository) :
    m_repository_ptr(_repository)
{
}

std::shared_ptr<Session> ServerRequestHandler::createSession(boost::asio::ip::tcp::socket _socket)
{
    return std::make_shared<SmtpSession>(std::move(_socket), m_repository_ptr);
}

bool ServerRequestHandler::handleError(const boost::system::error_code & _err_code)
{
    LOG_FATAL << "The SMTP server has stopped due an error: " << _err_code.message();
    return false;
}

SmtpSession::SmtpSession(TcpSocket _socket, std::shared_ptr<Repository> _repository) :
    TcpSession(std::move(_socket)),
    m_repository_ptr(_repository),
    mp_buffer(new char[s_buffer_size])
{
    LOG_DEBUG << "New SMTP session has started";
    mp_protocol = new Protocol(*m_repository_ptr, *this);
}

SmtpSession::~SmtpSession()
{
    LOG_DEBUG << "SMTP session has closed";
    delete [] mp_buffer;
    delete mp_protocol;
}

void SmtpSession::start()
{
    mp_protocol->start();
    callNextAction();
}

void SmtpSession::requestForRead()
{
    auto self(shared_from_this());
    readAsync(boost::asio::buffer(mp_buffer, s_buffer_size - 1),
        [self](const boost::system::error_code & ec, std::size_t length)
        {
            if(ec) return; // TODO: log
            self->mp_buffer[length] = '\0';
            self->mp_protocol->processInput(self->mp_buffer, length);
            // TODO: handle error
            self->callNextAction();
        });
}

void SmtpSession::requestForWrite(const Response & _response)
{
    std::stringstream data;
    data << _response << MU_SMTP_ENDLINE;
    auto self(shared_from_this());
    writeAsync(boost::asio::buffer(data.str()),
        [self](const boost::system::error_code &, std::size_t)
        {
            // TODO: handle error
            self->callNextAction();
        });
}

void SmtpSession::requestForSwitchToTls()
{
}

void SmtpSession::requestForExit()
{
    // Just do nothing
}
