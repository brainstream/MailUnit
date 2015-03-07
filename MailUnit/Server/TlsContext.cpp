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

#include <boost/algorithm/string.hpp>
#include <MailUnit/Server/TlsContext.h>

using namespace MailUnit::Server;
namespace fs = boost::filesystem;
namespace asio = boost::asio;

TlsContext::TlsContext(const TlsConfig & _config) :
    context(tlsv12_server)
{
    set_options(
        asio::ssl::context::default_workarounds |
        asio::ssl::context::no_tlsv1 |
        asio::ssl::context::no_sslv2 |
        asio::ssl::context::no_sslv3);
    std::string pass = _config.password;
    set_password_callback([pass](size_t, context_base::password_purpose) {
        return pass;
    });
    if(!_config.certPath.empty())
        use_certificate_chain_file(_config.certPath.string());
    if(!_config.keyPath.empty())
        use_private_key_file(_config.keyPath.string(), asio::ssl::context::pem);
}
