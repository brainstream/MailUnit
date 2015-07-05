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

#ifndef __LIBMU_MAIL_H__
#define __LIBMU_MAIL_H__

#include <stdio.h>
#include <time.h>
#include "Def.h"

/**
 * @file
 * @brief API for working with e-mail messages.
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
 *
 *
 * @anchor rfc-address-id
 * @htmlinclude RFC/AddressSpec.html
 *
 *
 * @anchor rfc-message-id
 * @htmlinclude RFC/IdentificationFieldsSpec.html
 *
 *
 * @anchor rfc-datetime-id
 * @htmlinclude RFC/DateSpec.html
*/


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
 * @brief The "Content-Type" MIME header field.
 *
 * @htmlinclude RFC/ContentTypeSpec.html
 */
#define MU_MAILHDR_CONTENTTYPE "Content-Type"

MU_DECLARE_HANDEL(MU_MAIL_HEADER);
MU_DECLARE_HANDEL(MU_MAIL_HEADERLIST);
MU_DECLARE_HANDEL(MU_MAILBOXGROUP);
MU_DECLARE_HANDEL(MU_MAILBOX);
MU_DECLARE_HANDEL(MU_MSGID);
MU_DECLARE_HANDEL(MU_MIME_MESSAGE);

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
MU_EXPORT MU_MAIL_HEADERLIST MU_CALL muMailHeadersParseString(const char * _input);

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
MU_EXPORT MU_MAIL_HEADERLIST MU_CALL muMailHeadersParseFile(MU_NATIVE_FILE _input);

/**
 * @brief Returns a count of @a _headers.
 */
MU_EXPORT size_t MU_CALL muMailHeadersCount(MU_MAIL_HEADERLIST _headers);

/**
 * @brief Returns a header at @a _index position or @a NULL.
 * @remarks
 *     Returned handle must be destroyed by calling the @ref muFree function.
 * @sa muMailHeadersCount
 */
MU_EXPORT MU_MAIL_HEADER MU_CALL muMailHeaderByIndex(MU_MAIL_HEADERLIST _headers, size_t _index);

/**
 * @brief Returns a header named @a _name or @a NULL.
 * @remarks
 *     Returned handle must be destroyed by calling the @ref muFree function.
 */
MU_EXPORT MU_MAIL_HEADER MU_CALL muMailHeaderByName(MU_MAIL_HEADERLIST _headers, const char * _name);

/**
 * @brief Returns a count of values in the @a _header.
 *
 * According with the table in
 * <A HREF="http://tools.ietf.org/html/rfc5322#section-3.6">RFC 5322 3.6</A> some of the headers
 * can appear more than once.
 *
 * @sa muMailHeaderValue
 */
MU_EXPORT size_t MU_CALL muMailHeaderValueCount(MU_MAIL_HEADER _header);

/**
 * @brief Returns a value of @a _header at @a _index position or @a NULL.
 *
 * According with the table in
 * <A HREF="http://tools.ietf.org/html/rfc5322#section-3.6">RFC 5322 3.6</A> some of the headers
 * can appear more than once.
 *
 * @sa muMailHeaderValueCount
*/
MU_EXPORT const char * MU_CALL muMailHeaderValue(MU_MAIL_HEADER _header, size_t _index);

/**
 * @brief Parses string described in @ref rfc-address-id "RFC"
 *
 * @param _raw_address_group
 *     String that describes either a mailbox or a mailbox group.
 * @return
 *     If string parsed successfully function returns a handle of the mailbox group object.
 *     In failure case the function returns @a MU_INVALID_HANDLE.
 * @remarks
 *     Returned handle must be destroyed by calling the @ref muFree function.
 */
MU_EXPORT MU_MAILBOXGROUP MU_CALL muMailboxGroupParse(const char * _raw_address_group);

/**
 * @brief Returns a count of mailboxes in @a _mailbox_group.
 */
MU_EXPORT size_t MU_CALL muMailboxCount(MU_MAILBOXGROUP _mailbox_group);

/**
 * @brief Returns a mailbox at @a _index position in @a _mailbox_group or @a MU_INVALID_HANDLE.
 * @remarks
 *     Returned handle must be destroyed by calling the @ref muFree function.
 */
MU_EXPORT MU_MAILBOX MU_CALL muMailbox(MU_MAILBOXGROUP _mailbox_group, size_t _index);

/**
 * @brief Returns the optional name of the mailbox group.
 *
 * If @a _mailbox_group does not contain a name, function returns @a NULL.
 */
MU_EXPORT const char * MU_CALL muMailboxGroupName(MU_MAILBOXGROUP _mailbox_group);

/**
 * @brief Returns the optional name of the mailbox.
 *
 * If @a _mailbox does not contain a name, function returns @a NULL.
 */
MU_EXPORT const char * MU_CALL muMailboxName(MU_MAILBOX _mailbox);

/**
 * @brief Returns an address from @a _mailbox or @a NULL.
 */
MU_EXPORT const char * MU_CALL muMailboxAddress(MU_MAILBOX _mailbox);

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

/**
 * @brief The month
 */
typedef enum
{
    mmonth_invalid = 0,  /**< The invalid month value */
    mmonth_jan     = 1,  /**< January */
    mmonth_feb     = 2,  /**< February */
    mmonth_mar     = 3,  /**< March */
    mmonth_apr     = 4,  /**< April */
    mmonth_may     = 5,  /**< May */
    mmonth_jun     = 6,  /**< June */
    mmonth_jul     = 7,  /**< July */
    mmonth_aug     = 8,  /**< August */
    mmonth_sep     = 9,  /**< September */
    mmonth_oct     = 10, /**< October */
    mmonth_nov     = 11, /**< November */
    mmonth_dec     = 12  /**< December */
} MMonth;

/**
 * @brief The day of week
 */
typedef enum
{
    mdow_invalid = 0, /**< The invalid day of week value */
    mdow_mon     = 1, /**< Monday */
    mdow_tue     = 2, /**< Tuesday */
    mdow_wed     = 3, /**< Wednesday */
    mdow_thu     = 4, /**< Thursday */
    mdow_fri     = 5, /**< Friday */
    mdow_sat     = 6, /**< Saturday */
    mdow_sun     = 7  /**< Sunday */
} MDayOfWeek;

/**
 * @brief Representation of date and time.
 */
typedef struct
{
    MDayOfWeek day_of_week;        /**< Day of week */
    unsigned short year;           /**< Year */
    MMonth month;                  /**< Mont */
    unsigned short day;            /**< Day */
    unsigned short hours;          /**< Hours */
    unsigned short minutes;        /**< Minutes */
    unsigned short seconds;        /**< Seconds */
    short timezone_offset_hours;   /**< Hours in timezone offset. Can have a negative value. */
    short timezone_offset_minutes; /**< Minutes in timezone offset. Can have a negative value. */
} MDateTime;

/**
 * @brief Parses string described in @ref rfc-datetime-id "RFC"
 * @param _raw_date_time
 *     String from a mail header.
 * @param _date_time
 *     A pointer to a variable that will contain a result. <i>Must not be NULL.</i>
 * @return
 *     In success case returns @ref MBool::mtrue and @ref MBool::mfalse otherwise.
 *     The @a _date_time object will not be modified if parsing failed.
 */
MU_EXPORT MBool MU_CALL muDateTimeParse(const char * _raw_date_time, MDateTime * _date_time);

/**
 * @brief Convert @ref MDateTime value to the UNIX time format
 * @param _date_time
 *     Value to convert.
 * @return
 *     The UNIX time representation of @a _date_time.
 * @sa muUnixTimeToDateTime
 */
MU_EXPORT time_t MU_CALL muDateTimeToUnixTime(const MDateTime * _date_time);

/**
 * @brief Convert UNIX time value to @ref MDateTime
 * @param _unix_time
 *     Date and time in the UNIX time format.
 * @param _date_time
 *     A pointer to a variable that will contain a result. <i>Must not be NULL.</i>
 * @sa muDateTimeToUnixTime
 */
MU_EXPORT void MU_CALL muUnixTimeToDateTime(time_t _unix_time, MDateTime * _date_time);


#endif // __LIBMU_MAIL_H__
