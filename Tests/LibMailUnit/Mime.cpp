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

#include <boost/test/unit_test.hpp>
#include <Include/LibMailUnit/Message/Mime.h>

namespace LibMailUnit {
namespace Test {

BOOST_AUTO_TEST_SUITE(Mime)

namespace {

static const char valid_message_src[] =
    "From: from@test.com\r\n"
    "To: to@test.com\r\n"
    "Subject: Test\r\n"
    "Content-Type: multipart/alternative; boundary=123456789\r\n"
    "\r\n"
    "This is a multi-part message in MIME format.\r\n"
    "\r\n"
    "--123456789\r\n"
    "Content-Type: text/plain\r\n"
    "\r\n"
    "The plain text format\r\n"
    "\r\n"
    "--123456789\r\n"
    "Content-Type: text/html\r\n"
    "\r\n"
    "<html>\r\n"
    "<head>\r\n"
    "<title>Test</title>\r\n"
    "</head>\r\n"
    "<body>\r\n"
    "<p>The html format</p>\r\n"
    "</body>\r\n"
    "--123456789--";

} // namespace

BOOST_AUTO_TEST_CASE(parseMimeStringTest)
{
    MU_MIME_MESSAGE message = muMimeParseString(valid_message_src);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, message);
    BOOST_CHECK_EQUAL("Test", muMimeSubject(message));
    size_t part_count = muMimePartCount(message);
    BOOST_CHECK_EQUAL(2, part_count);
    MU_MAIL_HEADERLIST headers = muMimeHeaders(message);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, headers);
    BOOST_CHECK_EQUAL(4, muMailHeadersCount(headers));
    MU_MAIL_HEADER header = muMailHeaderByIndex(headers, 0);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, header);
    BOOST_CHECK_EQUAL("From", muMailHeaderName(header));
    BOOST_CHECK_EQUAL(1, muMailHeaderValueCount(header));
    BOOST_CHECK_EQUAL("from@test.com", muMailHeaderValue(header, 0));
    muFree(header);
    header = muMailHeaderByIndex(headers, 1);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, header);
    BOOST_CHECK_EQUAL("To", muMailHeaderName(header));
    BOOST_CHECK_EQUAL(1, muMailHeaderValueCount(header));
    BOOST_CHECK_EQUAL("to@test.com", muMailHeaderValue(header, 0));
    muFree(header);
    header = muMailHeaderByIndex(headers, 2);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, header);
    BOOST_CHECK_EQUAL("Subject", muMailHeaderName(header));
    BOOST_CHECK_EQUAL(1, muMailHeaderValueCount(header));
    BOOST_CHECK_EQUAL("Test", muMailHeaderValue(header, 0));
    muFree(header);
    header = muMailHeaderByIndex(headers, 3);
    BOOST_CHECK_EQUAL("Content-Type", muMailHeaderName(header));
    BOOST_CHECK_EQUAL(1, muMailHeaderValueCount(header));
    BOOST_CHECK_EQUAL("multipart/alternative; boundary=123456789", muMailHeaderValue(header, 0));
    muFree(header);
    muFree(headers);

    MU_MIME_PART part = muMimePart(message, 0);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, part);
    headers = muMimePartHeaders(part);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, headers);
    BOOST_CHECK_EQUAL(1, muMailHeadersCount(headers));
    header = muMailHeaderByIndex(headers, 0);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, header);
    BOOST_CHECK_EQUAL(1, muMailHeaderValueCount(header));
    BOOST_CHECK_EQUAL("Content-Type", muMailHeaderName(header));
    BOOST_CHECK_EQUAL("text/plain", muMailHeaderValue(header, 0));
    muFree(header);
    muFree(headers);
    muFree(part);

    part = muMimePart(message, 1);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, part);
    headers = muMimePartHeaders(part);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, headers);
    BOOST_CHECK_EQUAL(1, muMailHeadersCount(headers));
    header = muMailHeaderByIndex(headers, 0);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, header);
    BOOST_CHECK_EQUAL(1, muMailHeaderValueCount(header));
    BOOST_CHECK_EQUAL("Content-Type", muMailHeaderName(header));
    BOOST_CHECK_EQUAL("text/html", muMailHeaderValue(header, 0));
    muFree(header);
    muFree(headers);
    muFree(part);

    muFree(message);
}


BOOST_AUTO_TEST_SUITE_END()

} // namespace LibMailUnit
} // namespace Test
