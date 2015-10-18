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

/**
 * @file
 * @brief API for working with MIME.
*/


#ifndef __LIBMU_MIME_H__
#define __LIBMU_MIME_H__

#include "../Def.h"
#include "MailHeader.h"
#include "Mailbox.h"
#include "ContentType.h"

#ifdef __cplusplus
extern "C" {
#endif

MU_DECLARE_API_TYPE(MU_MimeMessage)
MU_DECLARE_API_TYPE(MU_MimePart)

typedef enum
{
    mu_mbox_from,
    mu_mbox_to,
    mu_mbox_cc,
    mu_mbox_bcc
} MU_MailboxType;

MU_API const MU_MimeMessage * MU_CALL muMimeParseString(const char * _input);

MU_API const MU_MimeMessage * MU_CALL muMimeParseFile(MU_NATIVE_FILE _input);

MU_API const char * MU_CALL muMimeSubject(const MU_MimeMessage * _message);

MU_API size_t MU_CALL muMimeMailboxGroupCount(const MU_MimeMessage * _message, MU_MailboxType _mailbox_type);

MU_API const MU_MailboxGroup * MU_CALL muMimeMailboxGroup(const MU_MimeMessage * _message, MU_MailboxType _mailbox_type, size_t _index);

MU_API const MU_MimePart * MU_CALL muMimeToPart(const MU_MimeMessage * _message);

MU_API size_t MU_CALL muMimePartCount(const MU_MimePart * _part);

MU_API const MU_MimePart * MU_CALL muMimePart(const MU_MimePart * _part, size_t _index);

MU_API const MU_MailHeaderList * MU_CALL muMimeHeaders(const MU_MimePart * _part);

MU_API const MU_MailHeaderContentType * MU_CALL muMimeContentType(const MU_MimePart * _part);

MU_API const char * MU_CALL muMimeContent(const MU_MimePart * _part);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __LIBMU_MIME_H__
