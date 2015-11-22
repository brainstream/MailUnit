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
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>
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

MimePart::MimePart(std::istream & _stream)
{
    parse(_stream);
}

MimePart::~MimePart()
{
    for(const MimePart * part : m_parts)
        delete part;
}

void MimePart::parse(std::istream & _stream)
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

void MimePart::parseText(std::istream & _stream)
{
    const size_t buffer_size = 1024;
    char buffer[buffer_size];
    while(_stream.read(buffer, buffer_size) || _stream.gcount())
        m_text_content.append(buffer, buffer + _stream.gcount());
}

void MimePart::parseImage(std::istream & _stream)
{
    parseText(_stream);
}

void MimePart::parseAudio(std::istream & _stream)
{
    parseText(_stream);
}

void MimePart::parseVideo(std::istream & _stream)
{
    parseText(_stream);
}

void MimePart::parseApplication(std::istream & _stream)
{
    parseText(_stream);
}

void MimePart::parseMultipart(std::istream & _stream)
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
    std::stringstream * body = new std::stringstream();
    std::string line;
    bool body_open = false;
    while(std::getline(_stream, line))
    {
        if(boost::starts_with(line, boundary_end))
        {
            if(body_open)
            {
                m_parts.push_back(new MimePart(*body));
                body_open = false;
            }
            break;
        }
        else if(boost::starts_with(line, boundary_start))
        {
            if(body_open)
            {
                m_parts.push_back(new MimePart(*body));
                delete body;
                body = new std::stringstream();
            }
            else
            {
                body_open = true;
            }
        }
        else if(body_open)
        {
            (*body) << line << "\n";
        }
    }
    if(body_open)
    {
        m_parts.push_back(new MimePart(*body));
    }
    delete body;
}

void MimePart::parseMessage(std::istream & _stream)
{
    // TODO: implement
    throw std::runtime_error("The message MIME is not supported yet");
}

MimeMessage::MimeMessage(std::istream & _stream) :
    MimePart(_stream)
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

MU_MimeMessage * MU_CALL muMimeParseString(const char * _input)
{
    std::stringstream stream(_input);
    const MimeMessage * message = new MimeMessage(stream);
    return new MU_MimeMessage(message, true);
}

MU_MimeMessage * MU_CALL muMimeParseFile(MU_File _input)
{
    boost::iostreams::file_descriptor fdesc(_input, boost::iostreams::never_close_handle);
    boost::iostreams::stream<boost::iostreams::file_descriptor> stream(fdesc);
    const MimeMessage * message = new MimeMessage(stream);
    return new MU_MimeMessage(message, true);
}

const char * MU_CALL muMimeSubject(MU_MimeMessage * _message)
{
    if(nullptr == _message)
        return nullptr;
    const MimeMessage * message = _message->pointer();
    return message->subject().c_str();
}

size_t MU_CALL muMimeMailboxGroupCount(MU_MimeMessage * _message, MU_MailboxType _mailbox_type)
{
    if(nullptr == _message)
        return 0;
    const MimeMessage * message = _message->pointer();
    switch(_mailbox_type)
    {
    case mu_mbox_from:
        return message->from().size();
    case mu_mbox_to:
        return message->to().size();
    case mu_mbox_cc:
        return message->cc().size();
    case mu_mbox_bcc:
        return message->bcc().size();
    default:
        return 0;
    }
}

MU_MailboxGroup * MU_CALL muMimeMailboxGroup(MU_MimeMessage * _message, MU_MailboxType _mailbox_type, size_t _index)
{
    if(nullptr == _message)
        return nullptr;
    const MimeMessage * message = _message->pointer();
    const std::vector<const MailboxGroup *> * groups = nullptr;
    switch(_mailbox_type)
    {
    case mu_mbox_from:
        groups = &message->from();
        break;
    case mu_mbox_to:
        groups = &message->to();
        break;
    case mu_mbox_cc:
        groups = &message->cc();
        break;
    case mu_mbox_bcc:
        groups = &message->bcc();
        break;
    }
    if(nullptr == groups || _index >= groups->size())
        return nullptr;
    return new MU_MailboxGroup((*groups)[_index], false);
}

MU_MimePart * MU_CALL muMimeToPart(MU_MimeMessage * _message)
{
    if(nullptr == _message)
        return nullptr;
    const MimeMessage * mime = _message->pointer();
    return new MU_MimePart(mime, false);
}

size_t MU_CALL muMimePartCount(MU_MimePart * _part)
{
    if(nullptr == _part)
        return 0;
    const MimePart * part = _part->pointer();
    return part->parts().size();
}

MU_MimePart * MU_CALL muMimePart(MU_MimePart * _part, size_t _index)
{
    if(nullptr == _part)
        return nullptr;
    const MimePart * part = _part->pointer();
    size_t count = part->parts().size();
    if(_index >= count)
        return nullptr;
    return new MU_MimePart(part->parts()[_index], false);
}

MU_MailHeaderList * MU_CALL muMimeHeaders(MU_MimePart * _part)
{
    if(nullptr == _part)
        return nullptr;
    const MimePart * part = _part->pointer();
    return new MU_MailHeaderList(&part->headers(), false);
}

MU_MailHeaderContentType * MU_CALL muMimeContentType(MU_MimePart * _part)
{
    if(nullptr == _part)
        return nullptr;
    const MimePart * part = _part->pointer();
    return new MU_MailHeaderContentType(&part->contentType(), false);
}

const char * MU_CALL muMimeContent(MU_MimePart * _part)
{
    if(nullptr == _part)
        return nullptr;
    const MimePart * part = _part->pointer();
    return part->text().c_str();
}

