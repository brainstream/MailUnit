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

#ifndef __MU_SERVER_REQUESTHANDLER_H__
#define __MU_SERVER_REQUESTHANDLER_H__

#include <memory>
#include <boost/noncopyable.hpp>
#include <boost/system/error_code.hpp>
#include <Include/LibMailUnit/Def.h>
#include <MailUnit/Server/Session.h>

namespace MailUnit {
namespace Server {

template<typename Socket>
class RequestHandler : private boost::noncopyable
{
public:
    virtual ~RequestHandler()
    {
    }

    virtual std::shared_ptr<Session> createSession(Socket _socket) = 0;

    virtual bool handleError(const boost::system::error_code & _err_code)
    {
        MU_UNUSED(_err_code);
        return false;
    }
}; // class RequestHandler

} // namespace Server
} // namespace MailUnit

#endif // __MU_SERVER_REQUESTHANDLER_H__
