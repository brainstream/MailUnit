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

#ifdef __cplusplus
extern "C" {
#endif

MU_DECLARE_HANDEL(MU_MIME_MESSAGE);
MU_DECLARE_HANDEL(MU_MIME_PART);

MU_API MU_MIME_MESSAGE MU_CALL muMimeParseString(const char * _input);

MU_API MU_MIME_MESSAGE MU_CALL muMimeParseFile(MU_NATIVE_FILE _input);

MU_API size_t MU_CALL muMimePartCount(MU_MIME_MESSAGE _message);

MU_API MU_MIME_PART MU_CALL muMimePart(MU_MIME_MESSAGE _message, size_t _index);

MU_API MU_MAIL_HEADERLIST MU_CALL muMimeHeaders(MU_MIME_MESSAGE _message);

MU_API MU_MAIL_HEADERLIST MU_CALL muMimePartHeaders(MU_MIME_PART _message);

MU_API const char * MU_CALL muMimeSubject(MU_MIME_MESSAGE);

typedef enum
{
    mb_from,
    mb_to,
    mb_cc,
    mb_bcc
} MMailboxType;

MU_API size_t MU_CALL muMimeMailboxGroupCount(MU_MIME_MESSAGE _message, MMailboxType _mailbox_type);

MU_API MU_MAILBOXGROUP MU_CALL muMimeMailboxGroup(MU_MIME_MESSAGE, MMailboxType _mailobx_type, size_t _index);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __LIBMU_MIME_H__
