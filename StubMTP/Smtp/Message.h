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

#ifndef __STUBMTP_SMTP_MESSAGE_H__
#define __STUBMTP_SMTP_MESSAGE_H__

#include <string>
#include <vector>

namespace StubMTP {
namespace Smtp {

struct Message
{
    std::string from;
    std::vector<std::string> to;
    std::string data;
}; // struct Message

} // namespace Smtp
} // namespace StubMTP

#endif // __STUBMTP_SMTP_MESSAGE_H__
