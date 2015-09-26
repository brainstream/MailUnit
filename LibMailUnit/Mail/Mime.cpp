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
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <LibMailUnit/Mail/Mime.h>
#include <Include/LibMailUnit/Message/MailHeader.h>

using namespace LibMailUnit::Mail;

#define CST_UNKNOWN "*"

#define CT_TEXT "text"
#define CST_TEXT_PLAIN "plain"

#define CT_IMAGE "image"
#define CST_IMAGE_JPEG "jpeg"
#define CST_IMAGE_PNG "png"
#define CST_IMAGE_GIF "gif"

#define CT_AUDIO "audio"
#define CST_AUDIO_BASIC "basic"

#define CT_VIDEO "video"
#define CST_VIDEO_MPEG "mpeg"

#define CT_APPLICATION "application"
#define CST_APPLICATION_OCTSTREAM "octet-stream"

#define CT_MULTIPART "multipart"
#define CST_MULTIPART_MIXED "mixed"
#define CST_MULTIPART_ALTERNATIVES "alternatives"
#define CST_MULTIPART_DIGEST "digest"
#define CST_MULTIPART_PARALLEL "parallel"

#define CT_MESSAGE "message"
#define CST_MESSAGE_RFC822 "rfc822"

MimeMessagePart::MimeMessagePart(std::istream & _stream)
{
    parse(_stream);
}

MimeMessagePart::~MimeMessagePart()
{
    for(const MimeMessagePart * part : m_parts)
        delete part;
}

void MimeMessagePart::parse(std::istream & _stream)
{
    HeaderMap * map = new HeaderMap();
    HeaderParser::parse(_stream, *map);
    m_headers_ptr.reset(map);
    Header * content_type_hdr = m_headers_ptr->find(MU_MAILHDR_CONTENTTYPE);
    if(content_type_hdr && !content_type_hdr->values.empty())
        m_content_type_ptr = parseContentType(content_type_hdr->values[0]);
    if(!m_content_type_ptr)
        m_content_type_ptr.reset(new ContentType { CT_TEXT, CST_TEXT_PLAIN });
    if(boost::iequals(CT_IMAGE, m_content_type_ptr->type))
        parseImage(_stream);
    else if(boost::iequals(CT_AUDIO, m_content_type_ptr->type))
        parseAudio(_stream);
    else if(boost::iequals(CT_VIDEO, m_content_type_ptr->type))
        parseVideo(_stream);
    else if(boost::iequals(CT_APPLICATION, m_content_type_ptr->type))
        parseApplication(_stream);
    else if(boost::iequals(CT_MULTIPART, m_content_type_ptr->type))
        parseMultipart(_stream);
    else if(boost::iequals(CT_MESSAGE, m_content_type_ptr->type))
        parseMessage(_stream);
    else
        parseText(_stream);
}

void MimeMessagePart::parseText(std::istream & _stream)
{
    const size_t buffer_size = 1024;
    char buffer[buffer_size];
    while(_stream.read(buffer, buffer_size) || _stream.gcount())
        m_text_content.append(buffer, buffer + _stream.gcount());
}

void MimeMessagePart::parseImage(std::istream & _stream)
{
    parseText(_stream);
}

void MimeMessagePart::parseAudio(std::istream & _stream)
{
    parseText(_stream);
}

void MimeMessagePart::parseVideo(std::istream & _stream)
{
    parseText(_stream);
}

void MimeMessagePart::parseApplication(std::istream & _stream)
{
    parseText(_stream);
}

void MimeMessagePart::parseMultipart(std::istream & _stream)
{
    std::vector<ContentTypeParam>::const_iterator boundary_it = std::find_if(m_content_type_ptr->params.cbegin(),
        m_content_type_ptr->params.cend(), [](const ContentTypeParam & param) {
            return boost::algorithm::iequals("boundary", param.name);
        });
    if(m_content_type_ptr->params.end() == boundary_it)
    {
        // TODO: error!
        return;
    }
    const std::string boundary_start = std::string("--") + boundary_it->value;
    const std::string boundary_end = boundary_start + "--";
    std::stringstream body;
    std::string line;
    bool body_open = false;
    while(std::getline(_stream, line))
    {
        if(boost::starts_with(line, boundary_end))
        {
            if(body_open)
            {
                m_parts.push_back(new MimeMessagePart(body));
                body_open = false;
            }
            break;
        }
        else if(boost::starts_with(line, boundary_start))
        {
            if(body_open)
            {
                m_parts.push_back(new MimeMessagePart(body));
                body = std::stringstream();
            }
            else
            {
                body_open = true;
            }
        }
        else if(body_open)
        {
            body << line << "\n";
        }
    }
    if(body_open)
    {
        m_parts.push_back(new MimeMessagePart(body));
    }
}

void MimeMessagePart::parseMessage(std::istream & _stream)
{
    // TODO: implement
    throw std::runtime_error("The message MIME is not supported yet");
}

MimeMessage::MimeMessage(std::istream & _stream) :
    MimeMessagePart(_stream)
{
    const Header * subject_header = headers().find(MU_MAILHDR_SUBJECT);
    if(subject_header && !subject_header->values.empty())
        m_subject = subject_header->values[0];
    parseAddresses(MU_MAILHDR_FROM, m_from_addresses);
    parseAddresses(MU_MAILHDR_TO, m_to_addresses);
    parseAddresses(MU_MAILHDR_CC, m_cc_addresses);
    parseAddresses(MU_MAILHDR_BCC, m_bcc_addresses);
}

MimeMessage::~MimeMessage()
{
    for(const auto & addresses : { m_from_addresses, m_to_addresses, m_cc_addresses, m_bcc_addresses })
    {
        for(auto address : addresses)
            delete address;
    }
}

void MimeMessage::parseAddresses(const char * _header_name, std::vector<const MailboxGroup *> & _out)
{
    const Header * header = headers().find(_header_name);
    if(!header || header->values.empty())
        return;
    for(const std::string & header_value : header->values)
        _out.push_back(new MailboxGroup(header_value));
}
