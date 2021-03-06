/***********************************************************************************************
 *                                                                                             *
 * This file is part of the MailUnit Library.                                                  *
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
#include <LibMailUnit/Api/Include/Message/ContentType.h>

namespace LibMailUnit {
namespace Test {

BOOST_AUTO_TEST_SUITE(ContentType)

BOOST_AUTO_TEST_CASE(parseContentType)
{
    MU_MailHeaderContentType * content_type =
        muContentTypeParse("multipart/mixed;boundary = ---___123456 ; X-test = \"quoted  string--!_\"");
    BOOST_CHECK(content_type);
    const char * type = nullptr;
    const char * subtype = nullptr;
    BOOST_CHECK_EQUAL(mu_true, muContentType(content_type, &type, &subtype));
    BOOST_CHECK_EQUAL("multipart", type);
    BOOST_CHECK_EQUAL("mixed", subtype);
    BOOST_CHECK_EQUAL(2, muContentTypeParamsCount(content_type));
    const char * param_name = nullptr;
    const char * param_value = nullptr;
    BOOST_CHECK_EQUAL(mu_true, muContentTypeParam(content_type, 0, &param_name, &param_value));
    BOOST_CHECK_EQUAL("boundary", param_name);
    BOOST_CHECK_EQUAL("---___123456", param_value);
    BOOST_CHECK_EQUAL(mu_true, muContentTypeParam(content_type, 1, &param_name, &param_value));
    BOOST_CHECK_EQUAL("X-test", param_name);
    BOOST_CHECK_EQUAL("quoted  string--!_", param_value);
}

BOOST_AUTO_TEST_CASE(parseContentType2)
{
    MU_MailHeaderContentType * content_type = muContentTypeParse("text/plain");
    BOOST_CHECK(content_type);
    const char * type = nullptr;
    const char * subtype = nullptr;
    BOOST_CHECK_EQUAL(mu_true, muContentType(content_type, &type, &subtype));
    BOOST_CHECK_EQUAL("text", type);
    BOOST_CHECK_EQUAL("plain", subtype);
    BOOST_CHECK_EQUAL(0, muContentTypeParamsCount(content_type));
}

BOOST_AUTO_TEST_CASE(parseContentType3)
{
    MU_MailHeaderContentType * content_type = muContentTypeParse("X-text/X-some_value");
    BOOST_CHECK(content_type);
    const char * type = nullptr;
    const char * subtype = nullptr;
    BOOST_CHECK_EQUAL(mu_true, muContentType(content_type, &type, &subtype));
    BOOST_CHECK_EQUAL("X-text", type);
    BOOST_CHECK_EQUAL("X-some_value", subtype);
    BOOST_CHECK_EQUAL(0, muContentTypeParamsCount(content_type));
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace LibMailUnit
} // namespace Test
