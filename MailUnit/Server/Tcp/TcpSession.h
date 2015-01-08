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

#ifndef __MU_SERVER_TCP_TCPSESSION_H__
#define __MU_SERVER_TCP_TCPSESSION_H__

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <MailUnit/Server/Session.h>
#include <MailUnit/Server/TlsContext.h>

namespace MailUnit {
namespace Server {

typedef boost::asio::ip::tcp::socket TcpSocket;

class TcpSession : public Session
{
private:
    typedef boost::asio::ssl::stream<TcpSocket &> TlsSocket;

public:
    typedef std::function<void(const boost::system::error_code &, size_t)> ReadCallback;
    typedef std::function<void(const boost::system::error_code &)> HandshakeCallback;

    typedef boost::asio::mutable_buffers_1 OutBuffer;

public:
    explicit TcpSession(boost::asio::io_service & _io_service);
    explicit TcpSession(TcpSocket _socket);
    virtual ~TcpSession();
    void writeAsync(const InBuffer & _buffer, WriteCallback _callback) override;
    void readAsync(const OutBuffer & _buffer, ReadCallback _callback);
    void switchToTlsAsync(TlsContext & _context, HandshakeCallback _callback);

protected:
    TcpSocket & tcpSocket()
    {
        return m_tcp_socket;
    }

private:
    TcpSocket m_tcp_socket;
    TlsSocket * mp_tls_socket;
}; // class TcpSession

} // namespace Server
} // namespace MailUnit

#endif // __MU_SERVER_TCP_TCPSESSION_H__
