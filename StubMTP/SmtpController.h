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

#ifndef __STUBMTP_SMTPCONTROLLER_H__
#define __STUBMTP_SMTPCONTROLLER_H__

#include <StubMTP/Smtp/SessionProvider.h>

namespace StubMTP {

class SmtpController : public Smtp::SessionProvider
{
public:
    explicit SmtpController(boost::asio::io_service & _io_service);
    void onMessageRecieved(const Smtp::Message & _message) override;
    void onFail() override;
}; // class SmtpController

} // namespace StubMTP

#endif // __STUBMTP_SMTPCONTROLLER_H__
