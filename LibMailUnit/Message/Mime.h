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

#ifndef __LIBMU_MESSAGE_MIME_H__
#define __LIBMU_MESSAGE_MIME_H__

#include <memory>
#include <istream>
#include <vector>
#include <boost/noncopyable.hpp>
#include <LibMailUnit/Message/Headers.h>
#include <LibMailUnit/Message/ContentType.h>
#include <LibMailUnit/Message/Mailbox.h>

namespace LibMailUnit {
namespace Message {

class MimePart : private boost::noncopyable
{
public:
    explicit MimePart(std::istream & _stream);
    virtual ~MimePart();

    const HeaderMap & headers() const
    {
        return *m_headers_ptr;
    }

    const ContentType & contentType() const
    {
        return *m_content_type_ptr;
    }

    const std::vector<const MimePart *> & parts() const
    {
        return m_parts;
    }

    const std::string & text() const
    {
        return m_text_content;
    }

private:
    void parse(std::istream & _stream);
    void parseText(std::istream & _stream);
    void parseImage(std::istream & _stream);
    void parseAudio(std::istream & _stream);
    void parseVideo(std::istream & _stream);
    void parseApplication(std::istream & _stream);
    void parseMultipart(std::istream & _stream);
    void parseMessage(std::istream & _stream);

private:
    std::shared_ptr<HeaderMap> m_headers_ptr;
    std::shared_ptr<ContentType> m_content_type_ptr;
    std::string m_text_content;
    std::vector<const MimePart *> m_parts;
}; // class MimePart

class MimeMessage final : public MimePart
{
public:
    explicit MimeMessage(std::istream & _stream);
    ~MimeMessage() override;

    const std::string & subject() const
    {
        return m_subject;
    }

    const std::vector<const MailboxGroup *> & from() const
    {
        return m_from_addresses;
    }

    const std::vector<const MailboxGroup *> & to() const
    {
        return m_to_addresses;
    }

    const std::vector<const MailboxGroup *> & cc() const
    {
        return m_cc_addresses;
    }

    const std::vector<const MailboxGroup *> & bcc() const
    {
        return m_bcc_addresses;
    }

private:
    void parseAddresses(const char * _header_name, std::vector<const MailboxGroup *> & _out);

private:
    std::string m_subject;
    std::vector<const MailboxGroup *> m_from_addresses;
    std::vector<const MailboxGroup *> m_to_addresses;
    std::vector<const MailboxGroup *> m_cc_addresses;
    std::vector<const MailboxGroup *> m_bcc_addresses;
}; // class MimeMessage

} // namespace Message
} // namespace LibMailUnit

#endif // __LIBMU_MESSAGE_MIME_H__
