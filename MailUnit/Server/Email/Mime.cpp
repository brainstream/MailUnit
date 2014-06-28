/***********************************************************************************************
 *                                                                                             *
 * This file is part of MailUnit.                                                               *
 *                                                                                             *
 * MailUnit is free software: you can redistribute it and/or modify it under the terms of       *
 * the GNU General Public License as published by the Free Software Foundation,                *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * MailUnit is distributed in the hope that it will be useful,  but WITHOUT ANY WARRANTY;       *
 * without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  *
 * See the GNU General Public License for more details.                                        *
 *                                                                                             *
 * You should have received a copy of the GNU General Public License along with MailUnit.       *
 * If not, see <http://www.gnu.org/licenses/>.                                                 *
 *                                                                                             *
 ***********************************************************************************************/

#include <sstream>
#include <MailUnit/Server/Email/Mime.h>

using namespace MailUnit;
using namespace MailUnit::Email;

const char * const HeaderKey::message_id   = "Message-ID";
const char * const HeaderKey::sender       = "Sender";
const char * const HeaderKey::from         = "From";
const char * const HeaderKey::to           = "To";
const char * const HeaderKey::cc           = "Cc";
const char * const HeaderKey::bcc          = "Bcc";
const char * const HeaderKey::reply_to     = "Reply-To";
const char * const HeaderKey::date         = "Date";
const char * const HeaderKey::subject      = "Subject";
const char * const HeaderKey::user_agent   = "User-Agent";
const char * const HeaderKey::mime_version = "MIME-Version";

Mime::Mime(const Smtp::Message & _message)
{
    initHeaderMap(_message);
    initFromHeaderMap(m_message_id, HeaderKey::message_id);
    initFromHeaderMap(m_subject, HeaderKey::subject);
    initFromHeaderMap(m_mime_version, HeaderKey::mime_version);
    initFromHeaderMap(m_sender, HeaderKey::sender);
    initFromHeaderMap(m_from, HeaderKey::from);
    initFromHeaderMap(m_to, HeaderKey::to);
    initFromHeaderMap(m_cc, HeaderKey::cc);
    initFromHeaderMap(m_bcc, HeaderKey::bcc);
    initFromHeaderMap(m_reply_to, HeaderKey::reply_to);
    initFromHeaderMap(m_date, HeaderKey::date);
    // TODO: other headers
    appendBccFromSmtpMessage(_message);
    // TODO: contents
}

void Mime::initHeaderMap(const Smtp::Message & _message)
{
    std::stringstream data_stream(_message.data);
    parseHeaders(data_stream, m_header_map);
}

void Mime::initFromHeaderMap(DateTimePtr & _date_time, const char * _key)
{
    std::string value;
    if(findFirstHeaderValue(_key, value))
        _date_time = parseDateTime(value);
}

void Mime::initFromHeaderMap(AddressGroupPtr & _address_group, const char * _key)
{
    std::string value;
    if(findFirstHeaderValue(_key, value))
        _address_group = AddressGroup::parse(value);
    if(nullptr == _address_group)
        _address_group = makeEmptyAddressGroupPtr();
}

void Mime::initFromHeaderMap(MessageIdPtr & _message_id, const char * _key)
{
    std::string value;
    if(findFirstHeaderValue(_key, value))
        _message_id = MessageIdPtr(new MessageId(value));
}

void Mime::initFromHeaderMap(std::string & _string, const char * _key)
{
    findFirstHeaderValue(_key, _string);
}

bool Mime::findFirstHeaderValue(const char * _key, std::string & _result)
{
    HeaderMap::const_iterator it = m_header_map.find(_key);
    if(m_header_map.end() != it)
    {
        _result = it->second[0];
        return true;
    }
    return false;
}

void Mime::appendBccFromSmtpMessage(const Smtp::Message & _message)
{
    for(const std::string & raw_to: _message.to)
    {
        AddressPtr address = Address::parse(raw_to);
        if(address &&
           !m_to->containsMailbox(address->mailbox()) &&
           !m_cc->containsMailbox(address->mailbox()) &&
           !m_bcc->containsMailbox(address->mailbox()))
        {
            m_bcc->append(address);
        }
    }
}
