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

#ifndef __MU_MQP_ERROR_H__
#define __MU_MQP_ERROR_H__

#include <ostream>

namespace MailUnit {
namespace Mqp {

enum class StatusCode
{
    Success      = 100,
    UnknowError  = 200,
    ParseError   = 201,
    StorageError = 202,
    Timeout      = 203
}; // enum class StatusCode


} // namespace Mqp
} // namespace MailUnit

inline std::ostream & operator << (std::ostream & _stream, MailUnit::Mqp::StatusCode _error_code)
{
    _stream << static_cast<int>(_error_code);
    return _stream;
}

#endif // __MU_MQP_ERROR_H__
