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
 * \file
 * \brief Representation of the "Message-ID" Mail/MIME header
 *
 * \anchor rfc-message-id
 * <A HREF="http://tools.ietf.org/html/rfc5322#section-3.6.4">RFC 5322 3.6.4</A>:
 * <BLOCKQUOTE><PRE>
 * message-id      =   "Message-ID:" msg-id CRLF
 * in-reply-to     =   "In-Reply-To:" 1*msg-id CRLF
 * references      =   "References:" 1*msg-id CRLF
 * msg-id          =   [CFWS] "<" id-left "@" id-right ">" [CFWS]
 * id-left         =   dot-atom-text / obs-id-left
 * id-right        =   dot-atom-text / no-fold-literal / obs-id-right
 * no-fold-literal =   "[" *dtext "]"
 * </PRE></BLOCKQUOTE>
*/

#ifndef __MU_MIME_MESSAGEID_H__
#define __MU_MIME_MESSAGEID_H__

#include "../Memory.h"

typedef MU_HANDLE MU_MSGID;

/**
 * \brief Parses string described in \ref rfc-message-id "RFC"
 * \param _raw_message_id
 *     Singlr string from the mail header.
 * \return
 *     Handle to parsed message id or \ref MU_INVALID_HANDLE.
 * \sa muMessageIdString
 * \sa muMessageIdLeft
 * \sa muMessageIdRight
 */
MUAPI MU_MSGID muParseMessageId(const char * _raw_message_id);

/**
 * \brief Returns source string passed to the \ref muParseMessageId function.
 * \param _msg_id
 *     Handle returned from the \ref muParseMessageId function.
 * \sa muParseMessageId
 * \sa muMessageIdLeft
 * \sa muMessageIdRight
 */
MUAPI const char * muMessageIdString(MU_MSGID _msg_id);

/**
 * \brief Returns left part of message id.
 * \param _msg_id
 *     Handle returned from the \ref muParseMessageId function.
 * \sa muParseMessageId
 * \sa muMessageIdString
 * \sa muMessageIdRight
 */
MUAPI const char * muMessageIdLeft(MU_MSGID _msg_id);

/**
 * \brief Returns right part of message id.
 * \param _msg_id
 *     Handle returned from the \ref muParseMessageId function.
 * \sa muParseMessageId
 * \sa muMessageIdString
 * \sa muMessageIdLeft
 */
MUAPI const char * muMessageIdRight(MU_MSGID _msg_id);

#endif // __MU_MIME_MESSAGEID_H__
