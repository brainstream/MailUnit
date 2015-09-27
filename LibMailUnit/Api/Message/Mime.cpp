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
    const MimeMessage * message = new MimeMessage(stream);
    return new MHandle(message, true);
}

MU_MIME_MESSAGE MU_CALL muMimeParseFile(MU_NATIVE_FILE _input)
{
    boost::iostreams::file_descriptor fdesc(_input, boost::iostreams::never_close_handle);
    boost::iostreams::stream<boost::iostreams::file_descriptor> stream(fdesc);
    const MimeMessage * message = new MimeMessage(stream);
    return new MHandle(message, true);
}

size_t MU_CALL muMimePartCount(MU_MIME_MESSAGE _message)
{
    if(nullptr == _message || MU_INVALID_HANDLE == _message)
        return 0;
    const MimeMessage * mime = _message->pointer<const MimeMessage>();
    return mime->parts().size();
}

MU_MIME_PART MU_CALL muMimePart(MU_MIME_MESSAGE _message, size_t _index)
{
    if(nullptr == _message || MU_INVALID_HANDLE == _message)
        return MU_INVALID_HANDLE;
    const MimeMessage * mime = _message->pointer<const MimeMessage>();
    size_t count = mime->parts().size();
    if(_index >= count)
        return MU_INVALID_HANDLE;
    return new MHandle(mime->parts()[_index], false);

}

MU_MAIL_HEADERLIST MU_CALL muMimeHeaders(MU_MIME_MESSAGE _message)
{
    if(nullptr == _message || MU_INVALID_HANDLE == _message)
        return MU_INVALID_HANDLE;
    const MimeMessage * mime = _message->pointer<const MimeMessage>();
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
    const MimeMessage * message = _message->pointer<const MimeMessage>();
    return message->subject().c_str();
}

MU_MAIL_HEADER_CONTENT_TYPE MU_CALL muMimeContentType(MU_MIME_MESSAGE _message)
{
    if(nullptr == _message || MU_INVALID_HANDLE == _message)
        return MU_INVALID_HANDLE;
    const MimeMessage * message = _message->pointer<const MimeMessage>();
    //
    //
    // TODO: Separate method for a part
    //
    //
    //const MimeMessagePart * message = _message->pointer<const MimeMessagePart>();
    return new MHandle(&message->contentType(), false);
}

MU_MAIL_HEADER_CONTENT_TYPE MU_CALL muMimePartContentType(MU_MIME_PART _message_part)
{
    if(nullptr == _message_part || MU_INVALID_HANDLE == _message_part)
        return MU_INVALID_HANDLE;
    const MimeMessagePart * part = _message_part->pointer<const MimeMessagePart>();
    return new MHandle(&part->contentType(), false);
}

size_t MU_CALL muMimeMailboxGroupCount(MU_MIME_MESSAGE _message, MMailboxType _mailbox_type)
{
    if(nullptr == _message || MU_INVALID_HANDLE == _message)
        return 0;
    const MimeMessage * message = _message->pointer<const MimeMessage>();
    switch(_mailbox_type)
    {
    case mb_from:
        return message->from().size();
    case mb_to:
        return message->to().size();
    case mb_cc:
        return message->cc().size();
    case mb_bcc:
        return message->bcc().size();
    default:
        return 0;
    }
}

MU_MAILBOXGROUP MU_CALL muMimeMailboxGroup(MU_MIME_MESSAGE _message, MMailboxType _mailbox_type, size_t _index)
{
    if(nullptr == _message || MU_INVALID_HANDLE == _message)
        return MU_INVALID_HANDLE;
    const MimeMessage * message = _message->pointer<const MimeMessage>();
    const std::vector<const MailboxGroup *> * groups = nullptr;
    switch(_mailbox_type)
    {
    case mb_from:
        groups = &message->from();
        break;
    case mb_to:
        groups = &message->to();
        break;
    case mb_cc:
        groups = &message->cc();
        break;
    case mb_bcc:
        groups = &message->bcc();
        break;
    }
    if(nullptr == groups || _index >= groups->size())
        return MU_INVALID_HANDLE;
    return new MHandle((*groups)[_index], false);
}

const char * MU_CALL muMimeContent(MU_MIME_MESSAGE _message)
{
    if(nullptr == _message || MU_INVALID_HANDLE == _message)
        return nullptr;
    const MimeMessage * message = _message->pointer<const MimeMessage>();
    return message->text().c_str();
}

const char * MU_CALL muMimePartContent(MU_MIME_PART _message_part)
{
    if(nullptr == _message_part || MU_INVALID_HANDLE == _message_part)
        return nullptr;
    const MimeMessagePart * part = _message_part->pointer<const MimeMessagePart>();
    return part->text().c_str();
}

