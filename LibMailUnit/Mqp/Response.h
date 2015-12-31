/***********************************************************************************************
 *                                                                                             *
 * This file is part of MailUnit Library.                                                      *
 *                                                                                             *
 * MailUnit Library is free software: you can redistribute it and/or modify it under the terms *
 * of the GNU Lesser General Public License as published by the Free Software Foundation,      *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * MailUnit Library is distributed in the hope that it will be useful, but WITHOUT ANY         *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  *
 * PURPOSE. See the GNU Lesser General Public License for more details.                        *
 *                                                                                             *
 * You should have received a copy of the GNU License General Public License along with        *
 * MailUnit Library. If not, see <http://www.gnu.org/licenses>.                                *
 *                                                                                             *
 ***********************************************************************************************/

#ifndef __LIBMU_MQP_RESPONSE_H__
#define __LIBMU_MQP_RESPONSE_H__

namespace LibMailUnit {
namespace Mqp {

enum class ResponseType
{
    error,
    matched,
    deleted
}; // enum class ResponseType

struct ResponseHeader
{
    ResponseType response_type;
    unsigned int status_code;
    unsigned int affected_count;
}; // struct ResponseHeader

} // namespace Mqp
} // namespace LibMailUnit

#endif // __LIBMU_MQP_RESPONSE_H__
