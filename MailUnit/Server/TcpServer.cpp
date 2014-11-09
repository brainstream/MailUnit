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

#include <MailUnit/Server/TcpServer.h>

using namespace MailUnit::Server;
namespace asio = boost::asio;

namespace {

class TcpServer : public std::enable_shared_from_this<TcpServer>
{
public:
    inline TcpServer(asio::io_service & _io_service,
        const asio::ip::tcp::endpoint & _endpoint,
        std::shared_ptr<TcpRequestHandler> _handler);
    void accept();

private:
    asio::ip::tcp::socket m_socket;
    asio::ip::tcp::acceptor m_acceptor;
    std::shared_ptr<TcpRequestHandler> m_handler_ptr;
}; // class TcpServer

} // namespace

TcpServer::TcpServer(asio::io_service & _io_service,
        const asio::ip::tcp::endpoint & _endpoint,
        std::shared_ptr<TcpRequestHandler> _handler) :
    m_socket(_io_service),
    m_acceptor(_io_service, _endpoint),
    m_handler_ptr(_handler)
{
}

void TcpServer::accept()
{
    if(nullptr == m_handler_ptr)
    {
        return;
    }
    auto self = shared_from_this();
    m_acceptor.async_accept(m_socket, [self](boost::system::error_code err_code)
    {
        if(err_code)
        {
            if(!self->m_handler_ptr->handleError(err_code))
                return;
        }
        else
        {
            self->m_handler_ptr->handleConnection(std::move(self->m_socket));
        }
        self->accept();
    });
}

void MailUnit::Server::startTcpServer(asio::io_service & _io_service,
    const asio::ip::tcp::endpoint & _endpoint,
    std::shared_ptr<TcpRequestHandler> _handler)
{
    std::make_shared<TcpServer>(_io_service, _endpoint, _handler)->accept();
}
