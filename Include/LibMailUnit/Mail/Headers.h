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
 * @brief Representation of the mail and MIME headers.
 *
 * @anchor rfc-headers
 *
 * <A HREF="http://tools.ietf.org/html/rfc5322#section-2.1">RFC 5322 2.1</A>:
 * <BLOCKQUOTE>
 * A message consists of header fields (collectively called "the header
 * section of the message") followed, optionally, by a body.  The header
 * section is a sequence of lines of characters with special syntax as
 * defined in this specification.  The body is simply a sequence of
 * characters that follows the header section and is separated from the
 * header section by an empty line (i.e., a line with nothing preceding
 * the CRLF).
 * </BLOCKQUOTE>
 *
 * <A HREF="http://tools.ietf.org/html/rfc5322#section-2.2">RFC 5322 2.2</A>:
 * <BLOCKQUOTE>
 * header field.s are lines beginning with a field name, followed by a
 * colon (":"), followed by a field body, and terminated by CRLF.
 * </BLOCKQUOTE>
 *
 * <A HREF="http://tools.ietf.org/html/rfc5322#section-2.2.3">RFC 5322 2.2.3</A>:
 * <BLOCKQUOTE>
 * <P>... the field body portion of a header field. can be split into a
 *  multiple-line representation; this is called "folding".  The general
 *  rule is that wherever this specification allows for folding white
 *  space (not simply WSP characters), a CRLF may be inserted before any
 *  WSP.</P>
 *  <P>... The process of moving from this folded multiple-line representation
 *  of a header field. to its single line representation is called
 *  "unfolding".  Unfolding is accomplished by simply removing any CRLF
 *  that is immediately followed by WSP.</P>
 * <BLOCKQUOTE>
 */

#ifndef __LIBMU_MAIL_HEADERS_H__
#define __LIBMU_MAIL_HEADERS_H__

#include <stdio.h>
#include "../Memory.h"

/**
 * @brief The "Message-ID" mail header field.
 *
 * @htmlinclude RFC/IdentificationFieldsSpec.html
 */
#define MU_MAILHDR_MESSAGEID "Message-ID"

/**
 * @brief The "In-Reply-To" mail header field.
 *
 * @htmlinclude RFC/IdentificationFieldsSpec.html
 */
#define MU_MAILHDR_INREPLYTO "In-Reply-To"

/**
 * @brief The "References" mail header field.
 *
 * @htmlinclude RFC/IdentificationFieldsSpec.html
 */
#define MU_MAILHDR_REFERENCES "References"

/**
 * @brief The "Date" mail header field.
 *
 * <A HREF="http://tools.ietf.org/html/rfc5322#section-3.6.1">RFC 5322 3.6.1</A>:
 * <BLOCKQUOTE><PRE>
 * orig-date       =   "Date:" date-time CRLF
 * </PRE></BLOCKQUOTE>
 * @htmlinclude RFC/DateSpec.html
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
 */
#define MU_MAILHDR_BCC "Bcc"

/**
 * @brief The "Subject" mail header field.
 *
 * <A HREF="http://tools.ietf.org/html/rfc5322#section-3.6.5">RFC 5322 3.6.5</A>:
 * <BLOCKQUOTE><PRE>
 * subject         =   "Subject:" unstructured CRLF
 * </PRE></BLOCKQUOTE>
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
 */
#define MU_MAILHDR_USERAGENT "User-Agent"

/**
 * @brief The "MIME-Version" MIME header field.
 *
 * <A HREF="http://tools.ietf.org/html/rfc2045#section-4">RFC 2045 4</A>:
 * <BLOCKQUOTE><PRE>
 * version := "MIME-Version" ":" 1*DIGIT "." 1*DIGIT
 * </PRE></BLOCKQUOTE>
 */
#define MU_MAILHDR_VERSION "MIME-Version"

/**
 * @brief Contains values of headers with common name.
 *
 * According with the table in
 * <A HREF="http://tools.ietf.org/html/rfc5322#section-3.6">RFC 5322 3.6</A> some of the headers
 * can appear more than once.
 */
typedef struct
{
    /**
     * @brief A name of the header
     */
    const char * const name;
    /**
     * @brief List of values.
     *
     * Containst @ref values_count elements.
     */
    const char ** const values;
    /**
     * @brief Count of @ref values
     */
    const size_t values_count;
} MMailHeader;


typedef MU_HANDLE MU_MAIL_HEADER;
typedef MU_HANDLE MU_MAIL_HEADERLIST;


/**
 * @brief Parses headers of mail message from string.
 * @param _input
 *     String contained mail header. Can contain full mail message. Headers must be splitted
 *     from a data by an empty line as described in the \ref rfc-headers "RFC".
 * @return
 *     Handle to headers list or @ref MU_INVALID_HANDLE.
 * @remarks
 *     Returned handle must be destroyed by calling the @ref muFree function.
 */
MUAPI MU_MAIL_HEADERLIST muMailHeadersParseString(const char * _input);

/**
 * @brief Parses headers of mail message from file.
 * @param _input
 *     File that contains mail header. Can contain full mail message. Headers must be splitted
 *     from a data by an empty line as described in the \ref rfc-headers "RFC".
 * @return
 *     Handle to headers list or @ref MU_INVALID_HANDLE.
 *
 * Function will read file from current position.
 * After execution a read position will be placed after end of a header section.
 *
 * @remarks
 *     Returned handle must be destroyed by calling the @ref muFree function.
 */
MUAPI MU_MAIL_HEADERLIST muMailHeadersParseFile(MU_NATIVE_FILE _input);

/**
 * @brief Returns a count of @a _headers.
 */
MUAPI size_t muMailHeadersCount(MU_MAIL_HEADERLIST _headers);

/**
 * @brief Returns a header at @a _index position or @a NULL.
 * @remarks
 *     Returned handle does not require destruction.
 * @sa muMailHeadersCount
 */
MUAPI MU_MAIL_HEADER muMailHeaderByIndex(MU_MAIL_HEADERLIST _headers, size_t _index);

/**
 * @brief Returns a header named @a _name or @a NULL.
 * @remarks
 *     Returned handle does not require destruction.
 */
MUAPI MU_MAIL_HEADER muMailHeaderByName(MU_MAIL_HEADERLIST _headers, const char * _name);

/**
 * @brief Returns a count of values in the @a _header.
 *
 * According with the table in
 * <A HREF="http://tools.ietf.org/html/rfc5322#section-3.6">RFC 5322 3.6</A> some of the headers
 * can appear more than once.
 *
 * @sa muMailHeaderValue
 */
MUAPI size_t muMailHeaderValueCount(MU_MAIL_HEADER _header);

/**
 * @brief Returns a value of @a _header at @a _index position or @a NULL.
 *
 * According with the table in
 * <A HREF="http://tools.ietf.org/html/rfc5322#section-3.6">RFC 5322 3.6</A> some of the headers
 * can appear more than once.
 *
 * @sa muMailHeaderValueCount
*/
MUAPI const char * muMailHeaderValue(MU_MAIL_HEADER _header, size_t _index);

#endif // __LIBMU_MAIL_HEADERS_H__
