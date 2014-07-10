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
#include <LibMailUnit/Mail/Headers.h>
#include "../TestFixture.h"

namespace LibMailUnit {
namespace Test {

BOOST_FIXTURE_TEST_SUITE(Headers, TestFixture)

BOOST_AUTO_TEST_CASE(ParseString)
{
    const std::string raw_headers =
        "Name:Simple value\r\n"
        "Name2: Value\r\n"
        "\twith folding\r\n"
        "Name3:Value\r\n"
        "\twith folding\r\n"
        " on two lines\r\n"
        "Name:Simple value 2\r\n"
        "\r\n"
        "Error:String beyond header";
    MU_MAIL_HEADERLIST headers = muMailHeadersParseString(raw_headers.c_str());
    BOOST_CHECK_NE(MU_INVALID_HANDLE, headers);
    BOOST_CHECK_EQUAL(3, muMailHeadersCount(headers));

    MU_MAIL_HEADER header = muMailHeaderByName(headers, "Name");
    BOOST_CHECK_NE(MU_INVALID_HANDLE, header);
    BOOST_CHECK_EQUAL(2, muMailHeaderValueCount(header));
    const char * value = muMailHeaderValue(header, 0);
    BOOST_CHECK_EQUAL("Simple value", value);
    value = muMailHeaderValue(header, 1);
    BOOST_CHECK_EQUAL("Simple value 2", value);

    header = muMailHeaderByName(headers, "Name2");
    BOOST_CHECK_NE(MU_INVALID_HANDLE, header);
    BOOST_CHECK_EQUAL(1, muMailHeaderValueCount(header));
    value = muMailHeaderValue(header, 0);
    BOOST_CHECK_EQUAL("Value\twith folding", value);

    header = muMailHeaderByName(headers, "Name3");
    BOOST_CHECK_NE(MU_INVALID_HANDLE, header);
    BOOST_CHECK_EQUAL(1, muMailHeaderValueCount(header));
    value = muMailHeaderValue(header, 0);
    BOOST_CHECK_EQUAL("Value\twith folding on two lines", value);

    muFree(headers);
}

BOOST_AUTO_TEST_CASE(ParseFileTest)
{
    const std::string raw_headers =
        "Name:Simple value\r\n"
        "Name2: Value\r\n"
        "\twith folding\r\n"
        "Name3:Value\r\n"
        "\twith folding\r\n"
        " on two lines\r\n"
        "Name:Simple value 2\r\n"
        "\r\n"
        "Error:String beyond header";
    MU_NATIVE_FILE file = createTempFile(raw_headers);
    MU_MAIL_HEADERLIST headers = muMailHeadersParseFile(file);
    releaseFile(file);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, headers);
    BOOST_CHECK_EQUAL(3, muMailHeadersCount(headers));

    MU_MAIL_HEADER header = muMailHeaderByName(headers, "Name");
    BOOST_CHECK_NE(MU_INVALID_HANDLE, header);
    BOOST_CHECK_EQUAL(2, muMailHeaderValueCount(header));
    const char * value = muMailHeaderValue(header, 0);
    BOOST_CHECK_EQUAL("Simple value", value);
    value = muMailHeaderValue(header, 1);
    BOOST_CHECK_EQUAL("Simple value 2", value);

    header = muMailHeaderByName(headers, "Name2");
    BOOST_CHECK_NE(MU_INVALID_HANDLE, header);
    BOOST_CHECK_EQUAL(1, muMailHeaderValueCount(header));
    value = muMailHeaderValue(header, 0);
    BOOST_CHECK_EQUAL("Value\twith folding", value);

    header = muMailHeaderByName(headers, "Name3");
    BOOST_CHECK_NE(MU_INVALID_HANDLE, header);
    BOOST_CHECK_EQUAL(1, muMailHeaderValueCount(header));
    value = muMailHeaderValue(header, 0);
    BOOST_CHECK_EQUAL("Value\twith folding on two lines", value);

    muFree(headers);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace Test
} // namespace LibMailUnit
