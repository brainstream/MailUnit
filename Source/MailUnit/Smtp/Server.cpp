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

#include <iostream>
#include <MailUnit/Smtp/Server.h>
#include <MailUnit/Application.h>

using namespace MailUnit::Smtp;

struct Server::ServerHolder
{
    std::shared_ptr<Server> server;
};

void Server::startNew(boost::asio::io_service & _io_service, uint16_t _port, std::shared_ptr<SessionProvider> _provider)
{
    Server * server = new Server(_io_service, _port, _provider);
    std::shared_ptr<Server> ptr = std::shared_ptr<Server>(server);
    ServerHolder * holder = new ServerHolder();
    holder->server = ptr;
    server->m_holder = holder;
    std::stringstream log_message;
    log_message << "Starting the server on port " << _port;
    app().log().info(log_message.str());
    server->accept();
}

Server::Server(boost::asio::io_service & _io_service, uint16_t _port, std::shared_ptr<SessionProvider> _provider) :
    m_holder(nullptr),
    m_shutdown(false),
    m_provider_ptr(_provider),
    m_acceptor(_io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), _port)),
    m_socket(_io_service)
{
}

void Server::accept()
{
    std::shared_ptr<SessionProvider> provider = m_provider_ptr;
    m_acceptor.async_accept(m_socket, [this, provider](boost::system::error_code err_code)
    {
        if(m_shutdown)
        {
            return;
        }
        if(err_code)
        {
            std::cerr << "Server error: " << err_code.message() << std::endl;
            shutdown();
        }
        else
        {
            provider->startNewSession(std::move(m_socket));
            accept();
        }
    });

}

void Server::shutdown()
{
    std::cout << "Shutting down...\n";
    if(m_shutdown.exchange(true))
    {
        std::cout << "Already shutdown\n";
        return;
    }
    delete m_holder.exchange(nullptr);
}
