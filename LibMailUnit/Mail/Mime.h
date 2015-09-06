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

#ifndef __LIBMU_MAIL_MIME_H__
#define __LIBMU_MAIL_MIME_H__

#include <memory>
#include <istream>
#include <vector>
#include <boost/noncopyable.hpp>
#include <LibMailUnit/Mail/Headers.h>
#include <LibMailUnit/Mail/ContentType.h>
#include <LibMailUnit/Mail/Mailbox.h>

namespace LibMailUnit {
namespace Mail {

class MimeMessagePart : private boost::noncopyable
{
public:
    explicit MimeMessagePart(std::istream & _stream);
    virtual ~MimeMessagePart();

    const HeaderMap & headers() const
    {
        return *m_headers_ptr;
    }

    const std::vector<const MimeMessagePart *> & parts() const
    {
        return m_parts;
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
    std::vector<const MimeMessagePart *> m_parts;
}; // class MimeMessagePart

class MimeMessage final : public MimeMessagePart
{
public:
    explicit MimeMessage(std::istream & _stream);
    ~MimeMessage() override;

    const std::string & subject() const
    {
        return m_subject;
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

} // namespace Mail
} // namespace LibMailUnit

#endif // __LIBMU_MAIL_MIME_H__
