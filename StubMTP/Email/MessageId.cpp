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

#include <StubMTP/Email/MessageId.h>

using namespace StubMTP::Email;

/*
 * RFC 5322 3.6.4 - http://tools.ietf.org/html/rfc5322#section-3.6.4
 *
 * message-id      =   "Message-ID:" msg-id CRLF
 * in-reply-to     =   "In-Reply-To:" 1*msg-id CRLF
 * references      =   "References:" 1*msg-id CRLF
 * msg-id          =   [CFWS] "<" id-left "@" id-right ">" [CFWS]
 * id-left         =   dot-atom-text / obs-id-left
 * id-right        =   dot-atom-text / no-fold-literal / obs-id-right
 * no-fold-literal =   "[" *dtext "]"
 */

MessageId::MessageId(const std::string & _id_string) :
    m_id_string(_id_string)
{
    size_t at_pos = m_id_string.find("@");
    if(std::string::npos != at_pos)
    {
        size_t left_pos = '<' == _id_string[0] ? 1 : 0;
        size_t left_len = left_pos ? at_pos - 1 : at_pos;
        size_t right_pos = at_pos + 1;
        size_t right_len = '>' == _id_string[_id_string.length() - 1] ?
            _id_string.length() - right_pos - 1 : std::string::npos;
        m_left = m_id_string.substr(left_pos, left_len);
        m_right = m_id_string.substr(right_pos, right_len);
    }
}
