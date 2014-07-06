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
#include <LibMailUnit/InternalMemory.h>
#include <LibMailUnit/Mail/MessageId.h>

using namespace LibMailUnit;

namespace {

class MessageId
{
public:
    MU_DEFAULT_COPY(MessageId)
    explicit MessageId(const char * _id_string);

public:
    const std::string & full() const
    {
        return m_id_string;
    }

    const std::string & left() const
    {
        return m_left;
    }

    const std::string & right() const
    {
        return m_right;
    }

private:
    std::string m_id_string;
    std::string m_left;
    std::string m_right;
}; // class MessageId

} // namespace

MessageId::MessageId(const char * _id_string) :
    m_id_string(_id_string)
{
    // TODO: error!
    size_t at_pos = m_id_string.find("@");
    if(std::string::npos != at_pos)
    {
        size_t left_pos = '<' == m_id_string[0] ? 1 : 0;
        size_t left_len = left_pos ? at_pos - 1 : at_pos;
        size_t right_pos = at_pos + 1;
        size_t right_len = '>' == m_id_string[m_id_string.length() - 1] ?
            m_id_string.length() - right_pos - 1 : std::string::npos;
        m_left = m_id_string.substr(left_pos, left_len);
        m_right = m_id_string.substr(right_pos, right_len);
    }
}

MU_MSGID muParseMessageId(const char * _raw_message_id)
{
    MU_HANDLE handle = makeHandle<MessageId, const char *>(_raw_message_id);
    return handle;
    // TODO: MU_INVALID_HANDLE on error
}

const char * muMessageIdString(MU_MSGID _msg_id)
{
    if(nullptr == _msg_id)
        return nullptr;
    return handlePointer<MessageId>(_msg_id)->full().c_str();
}

const char * muMessageIdLeft(MU_MSGID _msg_id)
{
    if(nullptr == _msg_id)
        return nullptr;
    return handlePointer<MessageId>(_msg_id)->left().c_str();
}

const char * muMessageIdRight(MU_MSGID _msg_id)
{
    if(nullptr == _msg_id)
        return nullptr;
    return handlePointer<MessageId>(_msg_id)->right().c_str();
}
