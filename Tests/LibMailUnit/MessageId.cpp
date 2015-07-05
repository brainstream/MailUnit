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
#include <Include/LibMailUnit/Mail.h>

namespace LibMailUnit {
namespace Test {

BOOST_AUTO_TEST_SUITE(MessageId)

BOOST_AUTO_TEST_CASE(ParseTest)
{
    const char valid_id[]     = "<539DD070.1070602@127.0.0.1>";
    const char invalid_id_1[] = "_539DD070.1070602@127.0.0.1>";
    const char invalid_id_2[] = "<539DD070.1070602@127.0.0.1_";
    const char invalid_id_3[] = "<539DD070.1070602_127.0.0.1>";
    MU_MSGID msg_id = muMessageIdParse(valid_id);
    BOOST_CHECK_NE(MU_INVALID_HANDLE, msg_id);
    BOOST_CHECK_EQUAL(valid_id, muMessageIdString(msg_id));
    BOOST_CHECK_EQUAL("539DD070.1070602", muMessageIdLeft(msg_id));
    BOOST_CHECK_EQUAL("127.0.0.1", muMessageIdRight(msg_id));
    muFree(msg_id);
    BOOST_CHECK_EQUAL(MU_INVALID_HANDLE, muMessageIdParse(invalid_id_1));
    BOOST_CHECK_EQUAL(MU_INVALID_HANDLE, muMessageIdParse(invalid_id_2));
    BOOST_CHECK_EQUAL(MU_INVALID_HANDLE, muMessageIdParse(invalid_id_3));
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace Test
} // namespace LibMailUnit
