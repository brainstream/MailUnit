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

#ifndef __MU_SERVER_TLSCONTEXT_H__
#define __MU_SERVER_TLSCONTEXT_H__

#include <string>
#include <boost/asio/ssl.hpp>
#include <boost/filesystem/path.hpp>

namespace MailUnit {
namespace Server {

struct TlsConfig
{
    boost::filesystem::path certPath;
    boost::filesystem::path keyPath;
    std::string password;
}; // struct TlsConfig

class TlsContext : public boost::asio::ssl::context
{
public:
    explicit TlsContext(const TlsConfig & _config);
}; // class TlsContext

} // namespace Server
} // namespace MailUnit

#endif // __MU_SERVER_TLSCONTEXT_H__
