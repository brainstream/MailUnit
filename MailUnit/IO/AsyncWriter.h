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

#ifndef __MU_IO_ASYNCWRITER_H__
#define __MU_IO_ASYNCWRITER_H__

#include <functional>
#include <boost/asio.hpp>

namespace MailUnit {
namespace IO {

class AsyncWriter
{
public:
    typedef boost::asio::const_buffers_1 InBuffer;
    typedef std::function<void(const boost::system::error_code &, size_t)> WriteCallback;

public:
    virtual ~AsyncWriter()
    {
    }

    virtual void writeAsync(const InBuffer & _buffer, WriteCallback _callback) = 0;
}; // class AsyncWriter

} // namespace IO
} // namespace MailUnit

#endif // __MU_IO_ASYNCWRITER_H__
