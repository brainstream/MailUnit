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

#ifndef __MU_IO_ASYNCLAMBDAWRITER_H__
#define __MU_IO_ASYNCLAMBDAWRITER_H__

#include <functional>
#include <ostream>
#include <MailUnit/IO/AsyncOperation.h>

namespace MailUnit {
namespace IO {

template<typename Socket>
class AsyncLambdaWriter : public AsyncOperation<Socket>
{
public:
    typedef std::function<void(std::ostream & _data_stream)> Lambda;

public:
    AsyncLambdaWriter(Lambda _lambda) :
        m_lambda(_lambda)
    {
    }

    void run(Socket & _socket, AsioCallback _callback) override;

private:
    Lambda m_lambda;
}; // class AsyncLambdaWriter

template<typename Socket>
void AsyncLambdaWriter<Socket>::run(Socket & _socket, AsioCallback _callback)
{
    boost::asio::streambuf streambuf;
    std::ostream stream(&streambuf);
    m_lambda(stream);
    boost::asio::async_write(_socket, streambuf,
        [&_callback](const boost::system::error_code & error_code, std::size_t) {
            callAsioCallback(_callback, error_code);
        }
    );
}

} // namespace IO
} // namespace MailUnit

#endif // __MU_IO_ASYNCLAMBDAWRITER_H__
