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
#include <MailUnit/OS/FileSystem.h>

using namespace MailUnit::OS;

namespace LibMailUnit {
namespace Test {

BOOST_AUTO_TEST_SUITE(Mime)

namespace {

static const char message_src[] =
    "From: From Person<from@example.com>,From2 Person<from2@example.com>\r\n"
    "To: To Team: To Person<to@example.com>,To2 Person<to2@example.com>\r\n"
    "CC: CC Person<cc@example.com>,CC2 Person<cc2@example.com>\r\n"
    "BCC: BCC Person<bcc@example.com>,BCC2 Person<bcc2@example.com>\r\n"
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
    "--123456789--\r\n";

void testHeaders(MU_MIME_MESSAGE _message);
void testMailboxes(MU_MIME_MESSAGE _message);
void testParts(MU_MIME_MESSAGE _message);
void testMessage(MU_MIME_MESSAGE _message);

void testMessage(MU_MIME_MESSAGE _message)
{
    BOOST_CHECK_NE(MU_INVALID_HANDLE, _message);
    BOOST_CHECK_EQUAL("Test", muMimeSubject(_message));
    testHeaders(_message);
    testMailboxes(_message);
    testParts(_message);
}

void testHeaders(MU_MIME_MESSAGE _message)
{
    MU_MAIL_HEADERLIST headers = muMimeHeaders(_message);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, headers);
    BOOST_CHECK_EQUAL(6, muMailHeadersCount(headers));
    MU_MAIL_HEADER header = muMailHeaderByIndex(headers, 0);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, header);
    BOOST_CHECK_EQUAL("From", muMailHeaderName(header));
    BOOST_CHECK_EQUAL(1, muMailHeaderValueCount(header));
    BOOST_CHECK_EQUAL("From Person<from@example.com>,From2 Person<from2@example.com>", muMailHeaderValue(header, 0));
    muFree(header);
    header = muMailHeaderByIndex(headers, 1);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, header);
    BOOST_CHECK_EQUAL("To", muMailHeaderName(header));
    BOOST_CHECK_EQUAL(1, muMailHeaderValueCount(header));
    BOOST_CHECK_EQUAL("To Team: To Person<to@example.com>,To2 Person<to2@example.com>", muMailHeaderValue(header, 0));
    muFree(header);
    header = muMailHeaderByIndex(headers, 2);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, header);
    BOOST_CHECK_EQUAL("CC", muMailHeaderName(header));
    BOOST_CHECK_EQUAL(1, muMailHeaderValueCount(header));
    BOOST_CHECK_EQUAL("CC Person<cc@example.com>,CC2 Person<cc2@example.com>", muMailHeaderValue(header, 0));
    muFree(header);
    header = muMailHeaderByIndex(headers, 3);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, header);
    BOOST_CHECK_EQUAL("BCC", muMailHeaderName(header));
    BOOST_CHECK_EQUAL(1, muMailHeaderValueCount(header));
    BOOST_CHECK_EQUAL("BCC Person<bcc@example.com>,BCC2 Person<bcc2@example.com>", muMailHeaderValue(header, 0));
    muFree(header);
    header = muMailHeaderByIndex(headers, 4);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, header);
    BOOST_CHECK_EQUAL("Subject", muMailHeaderName(header));
    BOOST_CHECK_EQUAL(1, muMailHeaderValueCount(header));
    BOOST_CHECK_EQUAL("Test", muMailHeaderValue(header, 0));
    muFree(header);
    header = muMailHeaderByIndex(headers, 5);
    BOOST_CHECK_EQUAL("Content-Type", muMailHeaderName(header));
    BOOST_CHECK_EQUAL(1, muMailHeaderValueCount(header));
    BOOST_CHECK_EQUAL("multipart/alternative; boundary=123456789", muMailHeaderValue(header, 0));
    muFree(headers);
}

void testMailboxes(MU_MIME_MESSAGE _message)
{
    BOOST_CHECK_EQUAL(1, muMimeMailboxGroupCount(_message, mb_from));
    MU_MAILBOXGROUP mb_group = muMimeMailboxGroup(_message, mb_from, 0);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, mb_group);
    BOOST_CHECK(nullptr == muMailboxGroupName(mb_group));
    BOOST_CHECK_EQUAL(2, muMailboxCount(mb_group));
    MU_MAILBOX mbox = muMailbox(mb_group, 0);
    BOOST_CHECK_EQUAL("From Person", muMailboxName(mbox));
    BOOST_CHECK_EQUAL("from@example.com", muMailboxAddress(mbox));
    muFree(mbox);
    mbox = muMailbox(mb_group, 1);
    BOOST_CHECK_EQUAL("From2 Person", muMailboxName(mbox));
    BOOST_CHECK_EQUAL("from2@example.com", muMailboxAddress(mbox));
    muFree(mbox);
    muFree(mb_group);

    BOOST_CHECK_EQUAL(1, muMimeMailboxGroupCount(_message, mb_to));
    mb_group = muMimeMailboxGroup(_message, mb_to, 0);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, mb_group);
    BOOST_CHECK_EQUAL("To Team", muMailboxGroupName(mb_group));
    BOOST_CHECK_EQUAL(2, muMailboxCount(mb_group));
    mbox = muMailbox(mb_group, 0);
    BOOST_CHECK_EQUAL("To Person", muMailboxName(mbox));
    BOOST_CHECK_EQUAL("to@example.com", muMailboxAddress(mbox));
    muFree(mbox);
    mbox = muMailbox(mb_group, 1);
    BOOST_CHECK_EQUAL("To2 Person", muMailboxName(mbox));
    BOOST_CHECK_EQUAL("to2@example.com", muMailboxAddress(mbox));
    muFree(mbox);
    muFree(mb_group);

    BOOST_CHECK_EQUAL(1, muMimeMailboxGroupCount(_message, mb_cc));
    mb_group = muMimeMailboxGroup(_message, mb_cc, 0);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, mb_group);
    BOOST_CHECK(nullptr == muMailboxGroupName(mb_group));
    BOOST_CHECK_EQUAL(2, muMailboxCount(mb_group));
    mbox = muMailbox(mb_group, 0);
    BOOST_CHECK_EQUAL("CC Person", muMailboxName(mbox));
    BOOST_CHECK_EQUAL("cc@example.com", muMailboxAddress(mbox));
    muFree(mbox);
    mbox = muMailbox(mb_group, 1);
    BOOST_CHECK_EQUAL("CC2 Person", muMailboxName(mbox));
    BOOST_CHECK_EQUAL("cc2@example.com", muMailboxAddress(mbox));
    muFree(mbox);
    muFree(mb_group);

    BOOST_CHECK_EQUAL(1, muMimeMailboxGroupCount(_message, mb_bcc));
    mb_group = muMimeMailboxGroup(_message, mb_bcc, 0);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, mb_group);
    BOOST_CHECK(nullptr == muMailboxGroupName(mb_group));
    BOOST_CHECK_EQUAL(2, muMailboxCount(mb_group));
    mbox = muMailbox(mb_group, 0);
    BOOST_CHECK_EQUAL("BCC Person", muMailboxName(mbox));
    BOOST_CHECK_EQUAL("bcc@example.com", muMailboxAddress(mbox));
    muFree(mbox);
    mbox = muMailbox(mb_group, 1);
    BOOST_CHECK_EQUAL("BCC2 Person", muMailboxName(mbox));
    BOOST_CHECK_EQUAL("bcc2@example.com", muMailboxAddress(mbox));
    muFree(mbox);
    muFree(mb_group);
}

void testParts(MU_MIME_MESSAGE _message)
{
    BOOST_CHECK_EQUAL(2, muMimePartCount(_message));
    MU_MIME_PART part = muMimePart(_message, 0);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, part);
    MU_MAIL_HEADERLIST headers = muMimePartHeaders(part);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, headers);
    BOOST_CHECK_EQUAL(1, muMailHeadersCount(headers));
    MU_MAIL_HEADER header = muMailHeaderByIndex(headers, 0);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, header);
    BOOST_CHECK_EQUAL(1, muMailHeaderValueCount(header));
    BOOST_CHECK_EQUAL("Content-Type", muMailHeaderName(header));
    BOOST_CHECK_EQUAL("text/plain", muMailHeaderValue(header, 0));
    muFree(header);
    muFree(headers);
    muFree(part);

    part = muMimePart(_message, 1);
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
}

} // namespace

BOOST_AUTO_TEST_CASE(parseMimeStringTest)
{
    MU_MIME_MESSAGE message = muMimeParseString(message_src);
    testMessage(message);
    muFree(message);
}

BOOST_AUTO_TEST_CASE(parseMimeFileTest)
{
    TempFile temp_file;
    temp_file.write(message_src);
    temp_file.seek(0, std::ios_base::beg);
    MU_MIME_MESSAGE message = muMimeParseFile(temp_file);
    testMessage(message);
    muFree(message);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace LibMailUnit
} // namespace Test
