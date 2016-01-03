/***********************************************************************************************
 *                                                                                             *
 * This file is part of the MailUnit Library.                                                  *
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

#ifndef __LIBMU_APIIMPL_MESSAGE_MESSAGEID_H__
#define __LIBMU_APIIMPL_MESSAGE_MESSAGEID_H__

#include <string>
#include <LibMailUnit/Api/Impl/ApiObject.h>
#include <LibMailUnit/Api/Include/Message/MessageId.h>

namespace LibMailUnit {
namespace Message {

struct MessageId
{
    std::string id_string;
    std::string left;
    std::string right;
}; // struct MessageId

} // namespace Message
} // namespace LibMailUnit

MU_DEFINE_API_TYPE(MU_MailMessageId, const LibMailUnit::Message::MessageId)

#endif // __LIBMU_APIIMPL_MESSAGE_MESSAGEID_H__
