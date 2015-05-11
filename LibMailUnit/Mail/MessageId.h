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
 * @brief The message id header format parser and coresponding data types.
 *
 * @anchor rfc-message-id
 * @htmlinclude RFC/IdentificationFieldsSpec.html
*/

#ifndef __LIBMU_MAIL_MESSAGEID_H__
#define __LIBMU_MAIL_MESSAGEID_H__

#include "../Memory.h"

MU_DECLARE_HANDEL(MU_MSGID);

/**
 * @brief Parses string described in @ref rfc-message-id "RFC"
 * @param _raw_message_id
 *     String from a mail header.
 * @return
 *     Handle to parsed message id or @ref MU_INVALID_HANDLE.
 * @remarks
 *     Returned handle must be destroyed by calling the @ref muFree function.
 * @sa muMessageIdString
 * @sa muMessageIdLeft
 * @sa muMessageIdRight
 */
MU_EXPORT MU_MSGID MU_CALL muMessageIdParse(const char * _raw_message_id);

/**
 * @brief Returns source string passed to the @ref muMessageIdParse function.
 * @param _msg_id
 *     Handle returned from the @ref muMessageIdParse function.
 * @sa muParseMessageId
 * @sa muMessageIdLeft
 * @sa muMessageIdRight
 */
MU_EXPORT const char * MU_CALL muMessageIdString(MU_MSGID _msg_id);

/**
 * @brief Returns left part of message id.
 * @param _msg_id
 *     Handle returned from the @ref muMessageIdParse function.
 * @sa muMessageIdParse
 * @sa muMessageIdString
 * @sa muMessageIdRight
 */
MU_EXPORT const char * MU_CALL muMessageIdLeft(MU_MSGID _msg_id);

/**
 * @brief Returns right part of message id.
 * @param _msg_id
 *     Handle returned from the @ref muMessageIdParse function.
 * @sa muMessageIdParse
 * @sa muMessageIdString
 * @sa muMessageIdLeft
 */
MU_EXPORT const char * MU_CALL muMessageIdRight(MU_MSGID _msg_id);

#endif /* __LIBMU_MAIL_MESSAGEID_H__ */
