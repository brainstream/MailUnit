/***********************************************************************************************
 *                                                                                             *
 * This file is part of StubMTP.                                                               *
 *                                                                                             *
 * StubMTP is free software: you can redistribute it and/or modify it under the terms of       *
 * the GNU General Public License as published by the Free Software Foundation,                *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * StubMTP is distributed in the hope that it will be useful,  but WITHOUT ANY WARRANTY;       *
 * without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  *
 * See the GNU General Public License for more details.                                        *
 *                                                                                             *
 * You should have received a copy of the GNU General Public License along with StubMTP.       *
 * If not, see <http://www.gnu.org/licenses/>.                                                 *
 *                                                                                             *
 ***********************************************************************************************/

#ifndef __STUBMTP_EMAIL_MESSAGEID_H__
#define __STUBMTP_EMAIL_MESSAGEID_H__

#include <memory>
#include <string>
#include <ostream>
#include <StubMTP/Aux.h>

/*
 * Most important RFC parts:
 *
 * RFC 5322 - Internet Message Format. Section 3.6.4 - Identification Fields
 *     http://tools.ietf.org/html/rfc5322#section-3.6.4
 */

namespace StubMTP {
namespace Email {

class MessageId
{
public:
    STUBMTP_DEFAULT_COPY(MessageId)
    explicit MessageId(const std::string & _id_string);

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

typedef std::shared_ptr<MessageId> MessageIdPtr;

} // namespace Email
} // namespace StubMTP

inline std::ostream & operator << (std::ostream & _stream, const StubMTP::Email::MessageId & _message_id)
{
    _stream << _message_id.full();
    return _stream;
}

inline std::ostream & operator << (std::ostream & _stream, const StubMTP::Email::MessageIdPtr & _message_id)
{
    if(nullptr != _message_id)
        _stream << _message_id->full();
    return _stream;
}

#endif // __STUBMTP_EMAIL_MESSAGEID_H__
