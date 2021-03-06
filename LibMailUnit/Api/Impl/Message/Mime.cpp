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

#include <sstream>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>
#include <LibMailUnit/Api/Impl/Message/Mailbox.h>
#include <LibMailUnit/Api/Impl/Message/Headers.h>
#include <LibMailUnit/Api/Impl/Message/ContentType.h>
#include <LibMailUnit/Api/Impl/Message/Mime.h>

using namespace LibMailUnit::Message;

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

