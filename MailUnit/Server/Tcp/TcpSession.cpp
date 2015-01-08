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

#include <MailUnit/Server/Tcp/TcpSession.h>

using namespace MailUnit::Server;
namespace asio = boost::asio;


TcpSession::TcpSession(boost::asio::io_service & _io_service) :
    m_tcp_socket(_io_service),
    mp_tls_socket(nullptr)
{
}

TcpSession::TcpSession(TcpSocket _socket) :
    m_tcp_socket(std::move(_socket)),
    mp_tls_socket(nullptr)
{
}

TcpSession::~TcpSession()
{
    delete mp_tls_socket;
}

void TcpSession::writeAsync(const InBuffer & _buffer, WriteCallback _callback)
{
    if(mp_tls_socket)
        mp_tls_socket->async_write_some(_buffer, _callback);
    else
        m_tcp_socket.async_send(_buffer, _callback);
}

void TcpSession::readAsync(const OutBuffer & _buffer, ReadCallback _callback)
{
    if(mp_tls_socket)
        mp_tls_socket->async_read_some(_buffer, _callback);
    else
        m_tcp_socket.async_receive(_buffer, _callback);
}

void TcpSession::switchToTlsAsync(TlsContext & _context, HandshakeCallback _callback)
{
    if(mp_tls_socket)
    {
        _callback(boost::system::error_code());
        return;
    }
    mp_tls_socket = new TlsSocket(m_tcp_socket, _context);
    mp_tls_socket->async_handshake(boost::asio::ssl::stream_base::server, _callback);
}
