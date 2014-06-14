/***********************************************************************************************
 *                                                                                             *
 * This file is part of StubMTP.                                                               *
 *                                                                                             *
 * StubMTP is free software: you can redistribute it and/or modify it under the terms of       *
 * the GNU General Public License as published by the Free Software Foundation,                *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * StubMTP is distributed in the hope that it will be useful,  but WITHOUT ANY WARRANTY;       *
 * without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  *
 * See the GNU General Public License for more details.                                        *
 *                                                                                             *
 * You should have received a copy of the GNU General Public License along with StubMTP.       *
 * If not, see <http://www.gnu.org/licenses/>.                                                 *
 *                                                                                             *
 ***********************************************************************************************/

#ifndef __STUBMTP_SMTP_SESSIONPROVIDER_H__
#define __STUBMTP_SMTP_SESSIONPROVIDER_H__

#include <memory>
#include <boost/asio.hpp>
#include <StubMTP/Smtp/Message.h>

namespace StubMTP {
namespace Smtp {

class SessionProvider : public std::enable_shared_from_this<SessionProvider>
{
public:
    explicit SessionProvider(boost::asio::io_service & _io_service); // TODO: delete service from args
    virtual ~SessionProvider();
    void startNewSession(boost::asio::ip::tcp::socket _socket);
    virtual void onMessageRecieved(const Message & _message) = 0;
    virtual void onFail() = 0;

private:
    boost::asio::io_service & mr_io_service;
}; // class SessionProvider

} // namespace Smtp
} // namespace StubMTP

#endif // __STUBMTP_SMTP_SESSIONPROVIDER_H__
