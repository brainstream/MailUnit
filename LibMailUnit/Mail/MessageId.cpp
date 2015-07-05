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

#include <string>
#include <LibMailUnit/Memory.h>
#include <Include/LibMailUnit/Mail.h>

namespace {

struct MessageId
{
    std::string id_string;
    std::string left;
    std::string right;
}; // struct MessageId

} // namespace

MU_MSGID MU_CALL muMessageIdParse(const char * _raw_message_id)
{
    MessageId * message_id = new MessageId();
    message_id->id_string = _raw_message_id;
    size_t at_pos = message_id->id_string.find("@");
    size_t lt_pos = message_id->id_string.find("<");
    size_t gt_pos = message_id->id_string.rfind(">");
    if(std::string::npos == at_pos || std::string::npos == lt_pos || std::string::npos == gt_pos)
    {
        delete message_id;
        return MU_INVALID_HANDLE;
    }
    size_t left_pos = lt_pos + 1;
    size_t left_len = left_pos ? at_pos - 1 : at_pos;
    size_t right_pos = at_pos + 1;
    size_t right_len = gt_pos - at_pos - 1;
    message_id->left = message_id->id_string.substr(left_pos, left_len);
    message_id->right = message_id->id_string.substr(right_pos, right_len);
    return new MHandle(message_id, true);
}

const char * MU_CALL muMessageIdString(MU_MSGID _msg_id)
{
    if(nullptr == _msg_id)
        return nullptr;
    return _msg_id->pointer<MessageId>()->id_string.c_str();
}

const char * MU_CALL muMessageIdLeft(MU_MSGID _msg_id)
{
    if(nullptr == _msg_id)
        return nullptr;
    return _msg_id->pointer<MessageId>()->left.c_str();
}

const char * MU_CALL muMessageIdRight(MU_MSGID _msg_id)
{
    if(nullptr == _msg_id)
        return nullptr;
    return _msg_id->pointer<MessageId>()->right.c_str();
}
