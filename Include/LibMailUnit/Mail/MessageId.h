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

typedef MU_HANDLE MU_MSGID;

/**
 * @brief Parses string described in @ref rfc-message-id "RFC"
 * @param _raw_message_id
 *     String from a mail header.
 * @return
 *     Handle to parsed message id or @ref MU_INVALID_HANDLE.
 * @sa muMessageIdString
 * @sa muMessageIdLeft
 * @sa muMessageIdRight
 */
MUAPI MU_MSGID muMessageIdParse(const char * _raw_message_id);

/**
 * @brief Returns source string passed to the @ref muMessageIdParse function.
 * @param _msg_id
 *     Handle returned from the @ref muMessageIdParse function.
 * @sa muParseMessageId
 * @sa muMessageIdLeft
 * @sa muMessageIdRight
 */
MUAPI const char * muMessageIdString(MU_MSGID _msg_id);

/**
 * @brief Returns left part of message id.
 * @param _msg_id
 *     Handle returned from the @ref muMessageIdParse function.
 * @sa muMessageIdParse
 * @sa muMessageIdString
 * @sa muMessageIdRight
 */
MUAPI const char * muMessageIdLeft(MU_MSGID _msg_id);

/**
 * @brief Returns right part of message id.
 * @param _msg_id
 *     Handle returned from the @ref muMessageIdParse function.
 * @sa muMessageIdParse
 * @sa muMessageIdString
 * @sa muMessageIdLeft
 */
MUAPI const char * muMessageIdRight(MU_MSGID _msg_id);

#endif // __LIBMU_MAIL_MESSAGEID_H__
