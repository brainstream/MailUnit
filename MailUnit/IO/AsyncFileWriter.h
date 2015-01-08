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

void writeFileAsync(AsyncWriter & _writer, std::shared_ptr<std::ifstream> _stream, AsioCallback _callback);

class AsyncFileWriter : public AsyncOperation
{
public:
    explicit AsyncFileWriter(std::shared_ptr<std::ifstream> _stream) :
        m_stream(_stream)
    {
    }

    void run(AsyncWriter & _writer, AsioCallback _callback) override
    {
        writeFileAsync(_writer, m_stream, _callback);
    }

private:
    std::shared_ptr<std::ifstream> m_stream;
}; // class AsyncFileWriter

} // namespace IO
} // namespace MailUnit


#endif // __MU_IO_ASYNCFILEWRITER_H__
