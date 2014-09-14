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

#ifndef __MU_MQP_SERVERREQUESTHANDLER_H__
#define __MU_MQP_SERVERREQUESTHANDLER_H__

#include <memory>
#include <boost/asio.hpp>
#include <MailUnit/Server/RequestHandler.h>
#include <MailUnit/Storage/Database.h>

namespace MailUnit {
namespace Mqp {

class ServerRequestHandler : public Server::RequestHandler<boost::asio::ip::tcp::socket>
{
public:
    explicit ServerRequestHandler(std::shared_ptr<MailUnit::Storage::Database> _database) :
        m_database_ptr(_database)
    {
    }

    void handleConnection(boost::asio::ip::tcp::socket _socket) override;
    bool handleError(const boost::system::error_code & _err_code) override;

private:
    std::shared_ptr<MailUnit::Storage::Database> m_database_ptr;
}; // class ServerRequestHandler

} // namespace Storage
} // namespace Mqp

#endif // __MU_MQP_SERVERREQUESTHANDLER_H__
