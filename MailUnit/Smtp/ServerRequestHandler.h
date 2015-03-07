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

#ifndef __MU_SMTP_SERVERREQUESTHANDLER_H__
#define __MU_SMTP_SERVERREQUESTHANDLER_H__

#include <memory>
#include <boost/asio.hpp>
#include <boost/optional.hpp>
#include <MailUnit/Config.h>
#include <MailUnit/Server/RequestHandler.h>
#include <MailUnit/Storage/Repository.h>

namespace MailUnit {
namespace Smtp {

class ServerRequestHandler : public MailUnit::Server::RequestHandler<boost::asio::ip::tcp::socket>
{
public:
    ServerRequestHandler(std::shared_ptr<MailUnit::Storage::Repository> _repository, const Config & _config);
    std::shared_ptr<Server::Session> createSession(boost::asio::ip::tcp::socket _socket) override;
    bool handleError(const boost::system::error_code & _err_code) override;

private:
    std::shared_ptr<MailUnit::Storage::Repository> m_repository_ptr;
    const Config & mr_config;
}; // class ServerRequestHandler

} // namespace Smtp
} // namespace MailUnit

#endif // __MU_SMTP_SERVERREQUESTHANDLER_H__
