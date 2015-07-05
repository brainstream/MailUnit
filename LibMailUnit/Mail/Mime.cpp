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
#include <Include/LibMailUnit/Mail.h>
#include <LibMailUnit/Mail/Mime.h>
#include <LibMailUnit/Memory.h>

using namespace LibMailUnit::Mail;

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
    m_headers_ptr = HeaderParser::parse(_stream);
    Header * content_type_hdr = m_headers_ptr->find(MU_MAILHDR_CONTENTTYPE);
    if(content_type_hdr)
    {
        // TODO: content_type = parseContentType();
    }
    else
    {
        // TODO: content_type = text/plain
    }
    // TODO: parse(_stream, content_type)
}

MimeMessage::MimeMessage(std::istream & _stream) :
    MimeMessagePart(_stream)
{
    // TODO: subject, to, from, cc = find header
}

MU_MIME_MESSAGE MU_CALL muMimeMessageParseString(const char * _input)
{
    std::stringstream stream(_input);
    return new MHandle(new MimeMessage(stream), true);
}

MU_MIME_MESSAGE MU_CALL muMimeMessageParseFile(MU_NATIVE_FILE _input)
{
    boost::iostreams::file_descriptor fdesc(_input, boost::iostreams::never_close_handle);
    boost::iostreams::stream<boost::iostreams::file_descriptor> stream(fdesc);
    return new MHandle(new MimeMessage(stream), true);
}
