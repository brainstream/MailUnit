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

#ifndef __MU_SMTP_PSERVER_H__
#define __MU_SMTP_PSERVER_H__

#include <cstdint>
#include <memory>
#include <atomic>
#include <boost/asio.hpp>
#include <LibMailUnit/Def.h>
#include <MailUnit/Smtp/SessionProvider.h>

namespace MailUnit {
namespace Smtp {

class Server final
{
    MU_DISABLE_COPY(Server)
    struct ServerHolder;

public:
    static void startNew(boost::asio::io_service & _io_service, uint16_t _port, std::shared_ptr<SessionProvider> _provider);
    ~Server() = default;

private:
    Server(boost::asio::io_service & _io_service, uint16_t _port, std::shared_ptr<SessionProvider> _provider);
    void accept();
    void shutdown();

private:
    std::atomic<ServerHolder *> m_holder;
    std::atomic_bool m_shutdown;
    std::shared_ptr<SessionProvider> m_provider_ptr;
    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::asio::ip::tcp::socket m_socket;
}; // class Server

} // namespace Smtp
} // namespace MailUnit

#endif // __MU_SMTP_SERVER_H__
