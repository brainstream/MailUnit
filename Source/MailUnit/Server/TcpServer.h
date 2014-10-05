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

#ifndef __MU_SERVER_TCPSERVER_H__
#define __MU_SERVER_TCPSERVER_H__

#include <memory>
#include <boost/asio.hpp>
#include <MailUnit/Server/RequestHandler.h>

namespace MailUnit {
namespace Server {

typedef RequestHandler<boost::asio::ip::tcp::socket> TcpRequestHandler;

void startTcpServer(boost::asio::io_service & _io_service,
    const boost::asio::ip::tcp::endpoint & _endpoint,
    std::shared_ptr<TcpRequestHandler> _handler);

} // namespace Server
} // namespace MailUnit

#endif // __MU_SERVER_TCPSERVER_H__
