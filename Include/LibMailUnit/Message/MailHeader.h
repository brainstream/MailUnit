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

#ifndef __LIBMU_MAILHEADER_H__
#define __LIBMU_MAILHEADER_H__

#include "../Def.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file
 * @brief API for working with message headers.
 *
 * @anchor rfc-headers
 * @htmlinclude RFC/HeaderSpec.html
 *
 * @remarks According with the table in
 * <A HREF="http://tools.ietf.org/html/rfc5322#section-3.6">RFC 5322 3.6</A> some of the headers
 * can appear more than once.
 */

/**
 * @brief The "Message-ID" mail header field.
 *
 * @htmlinclude RFC/IdentificationFieldsSpec.html
 * @ingroup mail_header
 */
#define MU_MAILHDR_MESSAGEID "Message-ID"

/**
 * @brief The "In-Reply-To" mail header field.
 *
 * @htmlinclude RFC/IdentificationFieldsSpec.html
 * @ingroup mail_header
 */
#define MU_MAILHDR_INREPLYTO "In-Reply-To"

/**
 * @brief The "References" mail header field.
 *
 * @htmlinclude RFC/IdentificationFieldsSpec.html
 * @ingroup mail_header
 */
#define MU_MAILHDR_REFERENCES "References"

/**
 * @brief The "Date" mail header field.
 *
 * <A HREF="http://tools.ietf.org/html/rfc5322#section-3.6.1">RFC 5322 3.6.1</A>:
 * <BLOCKQUOTE><PRE>
 * orig-date       =   "Date:" date_time CRLF
 * </PRE></BLOCKQUOTE>
 * @htmlinclude RFC/DateSpec.html
 * @ingroup mail_header
 */
#define MU_MAILHDR_DATE "Date"

/**
 * @brief The "From" mail header field.
 *
 * <A HREF="http://tools.ietf.org/html/rfc6854#section-2.1">RFC 6854 2.1</A>:
 * <BLOCKQUOTE><PRE>
 * from = "From:" (mailbox-list / address-list) CRLF
 * </PRE></BLOCKQUOTE>
 * @htmlinclude RFC/AddressSpec.html
 * @ingroup mail_header
 */
#define MU_MAILHDR_FROM "From"

/**
 * @brief The "Sender" mail header field.
 *
 * <A HREF="http://tools.ietf.org/html/rfc6854#section-2.1">RFC 6854 2.1</A>:
 * <BLOCKQUOTE><PRE>
 * sender = "Sender:" (mailbox / address) CRLF
 * </PRE></BLOCKQUOTE>
 * @htmlinclude RFC/AddressSpec.html
 * @ingroup mail_header
 */
#define MU_MAILHDR_SENDER "Sender"

/**
 * @brief The "Reply-To" mail header field.
 *
 * <A HREF="http://tools.ietf.org/html/rfc6854#section-2.1">RFC 6854 2.1</A>:
 * <BLOCKQUOTE><PRE>
 * reply-to = "Reply-To:" address-list CRLF
 * </PRE></BLOCKQUOTE>
 * @htmlinclude RFC/AddressSpec.html
 * @ingroup mail_header
 */
#define MU_MAILHDR_REPLYTO "Reply-To"

/**
 * @brief The "To" mail header field.
 *
 * <A HREF="http://tools.ietf.org/html/rfc5322#section-3.6.3">RFC 5322 3.6.3</A>:
 * <BLOCKQUOTE><PRE>
 * to              =   "To:" address-list CRLF
 * </PRE></BLOCKQUOTE>
 * @htmlinclude RFC/AddressSpec.html
 * @ingroup mail_header
 */
#define MU_MAILHDR_TO "To"

/**
 * @brief The "Cc" mail header field.
 *
 * <A HREF="http://tools.ietf.org/html/rfc5322#section-3.6.3">RFC 5322 3.6.3</A>:
 * <BLOCKQUOTE><PRE>
 * cc              =   "Cc:" address-list CRLF
 * </PRE></BLOCKQUOTE>
 * @htmlinclude RFC/AddressSpec.html
 * @ingroup mail_header
 */
#define MU_MAILHDR_CC "Cc"

/**
 * @brief The "Cc" mail header field.
 *
 * <A HREF="http://tools.ietf.org/html/rfc5322#section-3.6.3">RFC 5322 3.6.3</A>:
 * <BLOCKQUOTE><PRE>
 * bcc             =   "Bcc:" [address-list / CFWS] CRLF
 * </PRE></BLOCKQUOTE>
 * @htmlinclude RFC/AddressSpec.html
 * @remarks
 *     BCC addresses can be defined by SMTP without including in the mail header.
 * @ingroup mail_header
 */
#define MU_MAILHDR_BCC "Bcc"

/**
 * @brief The "Subject" mail header field.
 *
 * <A HREF="http://tools.ietf.org/html/rfc5322#section-3.6.5">RFC 5322 3.6.5</A>:
 * <BLOCKQUOTE><PRE>
 * subject         =   "Subject:" unstructured CRLF
 * </PRE></BLOCKQUOTE>
 * @ingroup mail_header
 */
#define MU_MAILHDR_SUBJECT "Subject"

/**
 * @brief The non-standard "Subject" mail header field.
 *
 * The "User-Agent" field is not standardized for mail headers but usually the
 * <A HREF="http://tools.ietf.org/html/rfc7231#section-5.5.3">HTTP specification</A> is used::
 * <BLOCKQUOTE><PRE>
 * User-Agent      = product *( RWS ( product / comment ) )
 * product         = token ["/" product-version]
 * product-version = token
 * </PRE></BLOCKQUOTE>
 * @ingroup mail_header
 */
#define MU_MAILHDR_USERAGENT "User-Agent"

/**
 * @brief The "MIME-Version" MIME header field.
 *
 * <A HREF="http://tools.ietf.org/html/rfc2045#section-4">RFC 2045 4</A>:
 * <BLOCKQUOTE><PRE>
 * version := "MIME-Version" ":" 1*DIGIT "." 1*DIGIT
 * </PRE></BLOCKQUOTE>
 * @ingroup mail_header
 */
#define MU_MAILHDR_VERSION "MIME-Version"

/**
 * @brief The "Content-Type" MIME header field.
 *
 * @htmlinclude RFC/ContentTypeSpec.html
 * @ingroup mail_header
 */
#define MU_MAILHDR_CONTENTTYPE "Content-Type"

MU_DECLARE_API_TYPE(MU_MailHeader)
MU_DECLARE_API_TYPE(MU_MailHeaderList)


/**
 * @brief Parses headers of mail message from string.
 * @param _input
 *     String contained mail header. Can contain full mail message. Headers must be splitted
 *     from a data by an empty line as described in the \ref rfc-headers "RFC".
 * @return
 *     Pointer to a header list or @a NULL.
 * @remarks
 *     Returned potinter must be destroyed by calling the @ref muFree function.
 * @ingroup mail_header
 */
MU_API const MU_MailHeaderList * MU_CALL muMailHeadersParseString(const char * _input);

/**
 * @brief Parses headers of mail message from file.
 * @param _input
 *     File that contains mail header. Can contain full mail message. Headers must be splitted
 *     from a data by an empty line as described in the \ref rfc-headers "RFC".
 * @return
 *     Potinter to a header list or @a NULL.
 *
 * Function will read file from current position.
 * After execution a read position will be placed after end of a header section.
 *
 * @remarks
 *     Returned potinter must be destroyed by calling the @ref muFree function.
 * @ingroup mail_header
 */
MU_API const MU_MailHeaderList * MU_CALL muMailHeadersParseFile(MU_NATIVE_FILE _input);

/**
 * @brief Returns a count of @a _headers.
 */
MU_API size_t MU_CALL muMailHeadersCount(const MU_MailHeaderList * _headers);

/**
 * @brief Returns a header at @a _index position or @a NULL.
 * @remarks
 *     Returned potinter must be destroyed by calling the @ref muFree function.
 * @sa muMailHeadersCount
 * @ingroup mail_header
 */
MU_API const MU_MailHeader * MU_CALL muMailHeaderByIndex(const MU_MailHeaderList * _headers, size_t _index);

/**
 * @brief Returns a header named @a _name or @a NULL.
 * @remarks
 *     Returned potinter must be destroyed by calling the @ref muFree function.
 * @ingroup mail_header
 */
MU_API const MU_MailHeader * MU_CALL muMailHeaderByName(const MU_MailHeaderList * _headers, const char * _name);

/**
 * @brief Returns a name of @a _header or @a NULL.
 * @sa muMailHeaderByIndex
 * @ingroup mail_header
*/
MU_API const char * MU_CALL muMailHeaderName(const MU_MailHeader * _header);

/**
 * @brief Returns a count of values in the @a _header.
 *
 * According with the table in
 * <A HREF="http://tools.ietf.org/html/rfc5322#section-3.6">RFC 5322 3.6</A> some of the headers
 * can appear more than once.
 *
 * @sa muMailHeaderValue
 * @ingroup mail_header
 */
MU_API size_t MU_CALL muMailHeaderValueCount(const MU_MailHeader * _header);

/**
 * @brief Returns a value of @a _header at @a _index position or @a NULL.
 *
 * According with the table in
 * <A HREF="http://tools.ietf.org/html/rfc5322#section-3.6">RFC 5322 3.6</A> some of the headers
 * can appear more than once.
 *
 * @sa muMailHeaderValueCount
 * @ingroup mail_header
*/
MU_API const char * MU_CALL muMailHeaderValue(const MU_MailHeader * _header, size_t _index);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __LIBMU_MAILHEADER_H__ */
