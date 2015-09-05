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
    size_t part_count = muMimePartCount(message);
    BOOST_CHECK_EQUAL(2, part_count);
}


BOOST_AUTO_TEST_SUITE_END()

} // namespace LibMailUnit
} // namespace Test
