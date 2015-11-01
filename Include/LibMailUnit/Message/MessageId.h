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
 * @brief API for working with the Message-Id message header.
 *
 * @anchor rfc-message-id
 * @htmlinclude RFC/IdentificationFieldsSpec.html
*/

#ifndef __LIBMU_MESSAGEID_H__
#define __LIBMU_MESSAGEID_H__

#include "../Def.h"

#ifdef __cplusplus
extern "C" {
#endif

MU_DECLARE_API_TYPE(MU_MailMessageId)

/**
 * @brief Parses string described in @ref rfc-message-id "RFC"
 * @param _raw_message_id
 *     String from a mail header.
 * @return
 *     Potinter to the parsed message id or @a NULL.
 * @remarks
 *     Returned potinter must be destroyed by calling the @ref muFree function.
 * @sa muMessageIdString
 * @sa muMessageIdLeft
 * @sa muMessageIdRight
 * @ingroup message_id
 */
MU_API const MU_MailMessageId * MU_CALL muMessageIdParse(const char * _raw_message_id);

/**
 * @brief Returns source string passed to the @ref muMessageIdParse function.
 * @param _msg_id
 *     Potinter returned from the @ref muMessageIdParse function.
 * @sa muParseMessageId
 * @sa muMessageIdLeft
 * @sa muMessageIdRight
 * @ingroup message_id
 */
MU_API const char * MU_CALL muMessageIdString(const MU_MailMessageId * _msg_id);

/**
 * @brief Returns left part of message id.
 * @param _msg_id
 *     Potinter returned from the @ref muMessageIdParse function.
 * @sa muMessageIdParse
 * @sa muMessageIdString
 * @sa muMessageIdRight
 * @ingroup message_id
 */
MU_API const char * MU_CALL muMessageIdLeft(const MU_MailMessageId * _msg_id);

/**
 * @brief Returns right part of message id.
 * @param _msg_id
 *     Potinter returned from the @ref muMessageIdParse function.
 * @sa muMessageIdParse
 * @sa muMessageIdString
 * @sa muMessageIdLeft
 * @ingroup message_id
 */
MU_API const char * MU_CALL muMessageIdRight(const MU_MailMessageId * _msg_id);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __LIBMU_MESSAGEID_H__ */
