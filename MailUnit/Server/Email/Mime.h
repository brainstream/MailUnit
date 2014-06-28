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

#ifndef __MU_EMAIL_MIME_H__
#define __MU_EMAIL_MIME_H__

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <MailUnit/Server/Aux.h>
#include <MailUnit/Server/Smtp/Message.h>
#include <MailUnit/Server/Email/Header.h>
#include <MailUnit/Server/Email/Address.h>
#include <MailUnit/Server/Email/DateTime.h>
#include <MailUnit/Server/Email/MessageId.h>

/*
 * Most important RFC parts:
 *
 * RFC 5322 - Internet Message Format. Section 3.6 - Field Definitions
 *     http://tools.ietf.org/html/rfc5322#section-3.6
 * RFC 6854 - Update to Internet Message Format to Allow Group Syntax
 * in the "From:" and "Sender:" Header Fields
 *     http://tools.ietf.org/html/rfc6854
 */

namespace MailUnit {
namespace Email {

// TODO: move to the separate class
struct MailContent
{
    std::string type;
    std::string encoding;
    HeaderMap all_headers;
    std::string content;
}; // struct MailContent


struct HeaderKey
{
    static const char * const message_id;
    static const char * const sender;
    static const char * const from;
    static const char * const to;
    static const char * const cc;
    static const char * const bcc;
    static const char * const reply_to;
    static const char * const date;
    static const char * const subject;
    static const char * const user_agent;
    static const char * const mime_version;
}; // struct HeaderKey


class Mime
{
    MU_DISABLE_COPY(Mime)

public:
    explicit Mime(const Smtp::Message & _message);

public:
    const MessageIdPtr messageId() const
    {
        return m_message_id;
    }

    const DateTimePtr date() const
    {
        return m_date;
    }

    const std::string & userAgent() const
    {
        return m_user_agent;
    }

    const std::string & mimeVersion() const
    {
        return m_mime_version;
    }

    const std::string & subject() const
    {
        return m_subject;
    }

    const AddressGroupPtr sender() const
    {
        return m_sender;
    }

    const AddressGroupPtr from() const
    {
        return m_from;
    }

    const AddressGroupPtr to() const
    {
        return m_to;
    }

    const AddressGroupPtr cc() const
    {
        return m_cc;
    }

    const AddressGroupPtr bcc() const
    {
        return m_bcc;
    }

    const AddressGroupPtr replyTo() const
    {
        return m_reply_to;
    }

    const HeaderMap & allHeaders() const
    {
        return m_header_map;
    }

private:
    void initHeaderMap(const Smtp::Message & _message);
    void initFromHeaderMap(DateTimePtr & _date_time, const char * _key);
    void initFromHeaderMap(AddressGroupPtr & _address_group, const char * _key);
    void initFromHeaderMap(MessageIdPtr & _message_id, const char * _key);
    void initFromHeaderMap(std::string & _string, const char * _key);
    bool findFirstHeaderValue(const char * _key,  std::string & _result);
    void appendBccFromSmtpMessage(const Smtp::Message & _message);

private:
    MessageIdPtr m_message_id;
    DateTimePtr m_date;
    std::string m_user_agent;
    std::string m_mime_version;
    std::string m_subject;
    AddressGroupPtr m_sender;
    AddressGroupPtr m_from;
    AddressGroupPtr m_to;
    AddressGroupPtr m_cc;
    AddressGroupPtr m_bcc;
    AddressGroupPtr m_reply_to;
    HeaderMap m_header_map;
    // TODO: return         http://tools.ietf.org/html/rfc5322#section-3.6.7
    // TODO: received       http://tools.ietf.org/html/rfc5322#section-3.6.7
    // TODO: resent-date    http://tools.ietf.org/html/rfc5322#section-3.6.6
    // TODO: resent-from    http://tools.ietf.org/html/rfc5322#section-3.6.6
    // TODO: resent-sender  http://tools.ietf.org/html/rfc5322#section-3.6.6
    // TODO: resent-to      http://tools.ietf.org/html/rfc5322#section-3.6.6
    // TODO: resent-cc      http://tools.ietf.org/html/rfc5322#section-3.6.6
    // TODO: resent-bcc     http://tools.ietf.org/html/rfc5322#section-3.6.6
    // TODO: resent-msg-id  http://tools.ietf.org/html/rfc5322#section-3.6.6
    // TODO: in-reply-to    http://tools.ietf.org/html/rfc5322#section-3.6.4
    // TODO: references     http://tools.ietf.org/html/rfc5322#section-3.6.4
    // TODO: comments       http://tools.ietf.org/html/rfc5322#section-3.6.5
    // TODO: keywords       http://tools.ietf.org/html/rfc5322#section-3.6.5
    // TODO: std::vector<MailContent> m_contents;
}; // class Mime

} // namespace Email
} // namespace MailUnit


#endif // __MU_EMAIL_MIME_H__
