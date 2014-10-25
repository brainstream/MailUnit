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

#ifndef __MU_IO_ASYNCFILEWRITER_H__
#define __MU_IO_ASYNCFILEWRITER_H__

#include <memory>
#include <fstream>
#include <MailUnit/IO/AsyncOperation.h>

namespace MailUnit {
namespace IO {

template<typename Socket>
void writeFileAsync(Socket & _socket, std::shared_ptr<std::ifstream> _stream, AsioCallback _callback)
{
    static const size_t chank_size = 1024;
    char chank[chank_size];
    size_t symbol_count = _stream->read(chank, chank_size).gcount();
    if(symbol_count == 0)
    {
        _callback(boost::system::error_code());
        return;
    }
    boost::asio::async_write(_socket, boost::asio::buffer(chank, symbol_count),
        [&_socket, _stream, &_callback](const boost::system::error_code & error_code, std::size_t) {
            if(error_code && !callAsioCallback(_callback, error_code))
                return;
            writeFileAsync(_socket, _stream, _callback);
        }
    );
}

template<typename Socket>
class AsyncFileWriter : public AsyncOperation<Socket>
{
public:
    explicit AsyncFileWriter(std::shared_ptr<std::ifstream> _stream) :
        m_stream(_stream)
    {
    }

    void run(Socket & _socket, AsioCallback _callback) override
    {
        writeFileAsync(_socket, m_stream, _callback);
    }

private:
    std::shared_ptr<std::ifstream> m_stream;
}; // class AsyncFileWriter

} // namespace IO
} // namespace MailUnit


#endif // __MU_IO_ASYNCFILEWRITER_H__
