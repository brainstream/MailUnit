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

#include <MailUnit/IO/AsyncLambdaWriter.h>

using namespace MailUnit::IO;

void AsyncLambdaWriter::run(AsyncWriter & _writer, AsioCallback _callback)
{
    boost::asio::streambuf streambuf;
    std::ostream stream(&streambuf);
    m_lambda(stream);
    _writer.writeAsync(streambuf.data(),
        [_callback](const boost::system::error_code & error_code, std::size_t) {
            callAsioCallback(_callback, error_code);
        }
    );
}
