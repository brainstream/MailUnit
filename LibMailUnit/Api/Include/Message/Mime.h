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

/**
 * @file
 * @brief API for working with MIME.
*/


#ifndef __LIBMU_PUBAPI_MIME_H__
#define __LIBMU_PUBAPI_MIME_H__

#include "../Def.h"
#include "MailHeader.h"
#include "Mailbox.h"
#include "ContentType.h"

#ifdef __cplusplus
extern "C" {
#endif

MU_DECLARE_API_TYPE(MU_MimeMessage)
MU_DECLARE_API_TYPE(MU_MimePart)

/**
 * @brief Type of mailobx address
 * @ingroup mime
 */
typedef enum
{
    mu_mbox_from, /**< The @a from address type */
    mu_mbox_to,   /**< The @a to address type */
    mu_mbox_cc,   /**< The @a cc address type */
    mu_mbox_bcc   /**< The @a bcc address type */
} MU_MailboxType;


/**
 * @brief Parses message in MIME format from string
 * @param _input
 *     String containing a message
 * @return
 *     Returns a pointer to the parsed MIME message or @a NULL
 * @remarks
 *     Pointer must be destroyed by calling the @ref muFree function
 * @ingroup mime
 */
MU_API MU_MimeMessage * MU_CALL muMimeParseString(const char * _input);

/**
 * @brief Parses message in MIME format from a file
 * @param _input
 *     File containing a message
 * @return
 *     Pointer to the parsed MIME message or @a NULL
 * @remarks
 *     Returned pointer must be destroyed by calling the @ref muFree function
 * @ingroup mime
 */
MU_API MU_MimeMessage * MU_CALL muMimeParseFile(MU_File _input);

/**
 * @brief Returns a message subject of @a _message
 * @ingroup mime
 */
MU_API const char * MU_CALL muMimeSubject(MU_MimeMessage * _message);

/**
 * @brief Returns a count of mailbox groups of @a _mailobx_type type form the @a _message
 * @sa mailbox
 * @ingroup mime
 */
MU_API size_t MU_CALL muMimeMailboxGroupCount(MU_MimeMessage * _message, MU_MailboxType _mailbox_type);

/**
 * @brief Returns a mailbox group
 * @param _message
 *     Parsed MIME message
 * @param _mailbox_type
 *     Type of mailbox to match
 * @param _index
 *     Zero-based index of group
 * @return
 *     Pointer to the mailbox or @a NULL
 * @remarks
 *     Returned pointer must be destroyed by calling the @ref muFree function
 * @sa muMimeMailboxGroupCount
 * @sa mailbox
 * @ingroup mime
 */
MU_API MU_MailboxGroup * MU_CALL muMimeMailboxGroup(MU_MimeMessage * _message, MU_MailboxType _mailbox_type, size_t _index);

/**
 * @brief Converts a @a MU_MimeMessage pointer to a @a MU_MimePart pointer
 *
 * A MIME message can be represented as a part of the @a multipart/\* message.
 * Most of MIME API functions use a @a MU_MimePart instead of @a MU_MimeMessage to provide universal algorithms.
 * @remarks
 *     Returned pointer must be destroyed by calling the @ref muFree function
 * @ingroup mime
 */
MU_API MU_MimePart * MU_CALL muMimeToPart(MU_MimeMessage * _message);

/**
 * @brief Returns a count of parts in @a _part
 * @sa muMimeToPart
 * @ingroup mime
 */
MU_API size_t MU_CALL muMimePartCount(MU_MimePart * _part);

/**
 * @brief Returns a part
 * @param _part
 *     A message or a part of message containing parts
 * @param _index
 *     Zero-based index of part to return
 * @returns
 *     Pointer to MIME part or @a NULL
 * @remarks
 *     Returned pointer must be destroyed by calling the @ref muFree function
 * @sa muMimeToPart
 * @ingroup mime
 */
MU_API MU_MimePart * MU_CALL muMimePart(MU_MimePart * _part, size_t _index);

/**
 * @brief Returns MIME part's headers
 * @param _part
 *     A message or a part of message containing headers
 * @returns
 *     Pointer to list of headers or @a NULL
 * @remarks
 *     Returned pointer must be destroyed by calling the @ref muFree function
 * @sa muMimeToPart
 * @sa mail_header
 * @ingroup mime
 */
MU_API MU_MailHeaderList * MU_CALL muMimeHeaders(MU_MimePart * _part);

/**
 * @brief Returns MIME part's content type
 * @param _part
 *     A message or a part of message containing headers
 * @returns
 *     Pointer to a content type or @a NULL
 * @remarks
 *     Returned pointer must be destroyed by calling the @ref muFree function
 * @sa muMimeToPart
 * @sa content_type
 * @ingroup mime
 */
MU_API MU_MailHeaderContentType * MU_CALL muMimeContentType(MU_MimePart * _part);

/**
 * @brief Returns content of @a _part or @a NULL
 * @sa muMimeToPart
 * @ingroup mime
 */
MU_API const char * MU_CALL muMimeContent(MU_MimePart * _part);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __LIBMU_PUBAPI_MIME_H__ */
