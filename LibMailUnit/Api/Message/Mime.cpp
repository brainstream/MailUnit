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

#include <sstream>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <LibMailUnit/Memory.h>
#include <LibMailUnit/Mail/Mime.h>
#include <Include/LibMailUnit/Message/Mime.h>

using namespace LibMailUnit::Mail;

MU_MIME_MESSAGE MU_CALL muMimeParseString(const char * _input)
{
    std::stringstream stream(_input);
    return new MHandle(new MimeMessage(stream), true);
}

MU_MIME_MESSAGE MU_CALL muMimeParseFile(MU_NATIVE_FILE _input)
{
    boost::iostreams::file_descriptor fdesc(_input, boost::iostreams::never_close_handle);
    boost::iostreams::stream<boost::iostreams::file_descriptor> stream(fdesc);
    return new MHandle(new MimeMessage(stream), true);
}

size_t MU_CALL muMimePartCount(MU_MIME_MESSAGE _message)
{
    if(nullptr == _message || MU_INVALID_HANDLE == _message)
        return 0;
    MimeMessage * mime = _message->pointer<MimeMessage>();
    return mime->parts().size();
}

MU_MIME_PART MU_CALL muMimePart(MU_MIME_MESSAGE _message, size_t _index)
{
    if(nullptr == _message || MU_INVALID_HANDLE == _message)
        return MU_INVALID_HANDLE;
    MimeMessage * mime = _message->pointer<MimeMessage>();
    size_t count = mime->parts().size();
    if(_index >= count)
        return MU_INVALID_HANDLE;
    return new MHandle(mime->parts()[_index], false);

}

MU_MAIL_HEADERLIST MU_CALL muMimeHeaders(MU_MIME_MESSAGE _message)
{
    if(nullptr == _message || MU_INVALID_HANDLE == _message)
        return MU_INVALID_HANDLE;
    MimeMessage * mime = _message->pointer<MimeMessage>();
    return new MHandle(&mime->headers(), false);
}

MU_MAIL_HEADERLIST MU_CALL muMimePartHeaders(MU_MIME_PART _message_part)
{
    if(nullptr == _message_part || MU_INVALID_HANDLE == _message_part)
        return MU_INVALID_HANDLE;
    const MimeMessagePart * part = _message_part->pointer<const MimeMessagePart>();
    return new MHandle(&part->headers(), false);
}

const char * MU_CALL muMimeSubject(MU_MIME_MESSAGE _message)
{
    if(nullptr == _message || MU_INVALID_HANDLE == _message)
        return nullptr;
    MimeMessage * message = _message->pointer<MimeMessage>();
    return message->subject().c_str();
}

