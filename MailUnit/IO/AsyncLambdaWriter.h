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

class AsyncLambdaWriter : public AsyncOperation
{
public:
    typedef std::function<void(std::ostream & _data_stream)> Lambda;

public:
    AsyncLambdaWriter(Lambda _lambda) :
        m_lambda(_lambda)
    {
    }

    void run(AsyncWriter &_writer, AsioCallback _callback) override;

private:
    Lambda m_lambda;
}; // class AsyncLambdaWriter

} // namespace IO
} // namespace MailUnit

#endif // __MU_IO_ASYNCLAMBDAWRITER_H__
