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

#ifndef __MU_IO_IODEF_H__
#define __MU_IO_IODEF_H__

#include <functional>
#include <boost/asio.hpp>

namespace MailUnit {
namespace IO {

typedef std::function<bool(const boost::system::error_code &)> AsioCallback;

inline bool callAsioCallback(AsioCallback & _callback,
    const boost::system::error_code & _error_code = boost::system::error_code())
{
    if(_callback == nullptr)
        return true;
    return _callback(_error_code);
}

} // namespace IO
} // namespace MailUnit

#endif // __MU_IO_IODEF_H__
