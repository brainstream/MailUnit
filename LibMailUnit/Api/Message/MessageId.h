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

#ifndef __LIBMU_API_MESSAGE_MESSAGEID_H__
#define __LIBMU_API_MESSAGE_MESSAGEID_H__

#include <string>
#include <LibMailUnit/Api/ApiObject.h>
#include <Include/LibMailUnit/Message/MessageId.h>

namespace LibMailUnit {
namespace Api {
namespace Message {

struct MessageId
{
    std::string id_string;
    std::string left;
    std::string right;
}; // struct MessageId

} // namespace Message
} // namespace Api
} // namespace LibMailUnit

MU_DEFINE_API_TYPE(MU_MailMessageId, LibMailUnit::Api::Message::MessageId)

#endif // __LIBMU_API_MESSAGE_MESSAGEID_H__
