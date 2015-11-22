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
#include <Include/LibMailUnit/Message/Mailbox.h>

namespace LibMailUnit {
namespace Test {

BOOST_AUTO_TEST_SUITE(Address)

BOOST_AUTO_TEST_CASE(ParseMailboxTest)
{
    const char text[] = " my.test@test.example.com ";
    MU_MailboxGroup * mailbox_group = muMailboxGroupParse(text);
    BOOST_CHECK(mailbox_group);
    BOOST_CHECK(nullptr == muMailboxGroupName(mailbox_group));
    BOOST_CHECK_EQUAL(1, muMailboxCount(mailbox_group));
    MU_Mailbox * mailbox = muMailbox(mailbox_group, 0);
    BOOST_CHECK(mailbox);
    BOOST_CHECK(nullptr == muMailboxName(mailbox));
    BOOST_CHECK_EQUAL("my.test@test.example.com", muMailboxAddress(mailbox));
    muFree(mailbox_group);
    muFree(mailbox);
}

BOOST_AUTO_TEST_CASE(ParseNamedMailboxTest)
{
    const char text[] = " The name <my.test@test.example.com> ";
    MU_MailboxGroup * mailbox_group = muMailboxGroupParse(text);
    BOOST_CHECK(mailbox_group);
    BOOST_CHECK(nullptr == muMailboxGroupName(mailbox_group));
    BOOST_CHECK_EQUAL(1, muMailboxCount(mailbox_group));
    MU_Mailbox * mailbox = muMailbox(mailbox_group, 0);
    BOOST_CHECK(mailbox);
    BOOST_CHECK_EQUAL("The name", muMailboxName(mailbox));
    BOOST_CHECK_EQUAL("my.test@test.example.com", muMailboxAddress(mailbox));
    muFree(mailbox_group);
    muFree(mailbox);
}

BOOST_AUTO_TEST_CASE(ParseMailboxGroupTest)
{
    const char text[] = " my.test@test.example.com, my.test.2@test.example.com ";
    MU_MailboxGroup * mailbox_group = muMailboxGroupParse(text);
    BOOST_CHECK(mailbox_group);
    BOOST_CHECK(nullptr == muMailboxGroupName(mailbox_group));
    BOOST_CHECK_EQUAL(2, muMailboxCount(mailbox_group));

    MU_Mailbox * mailbox = muMailbox(mailbox_group, 0);
    BOOST_CHECK(mailbox);
    BOOST_CHECK(nullptr == muMailboxName(mailbox));
    BOOST_CHECK_EQUAL("my.test@test.example.com", muMailboxAddress(mailbox));
    muFree(mailbox);

    mailbox = muMailbox(mailbox_group, 1);
    BOOST_CHECK(mailbox);
    BOOST_CHECK(nullptr == muMailboxName(mailbox));
    BOOST_CHECK_EQUAL("my.test.2@test.example.com", muMailboxAddress(mailbox));
    muFree(mailbox);
    muFree(mailbox_group);
}

BOOST_AUTO_TEST_CASE(ParseNamedMailboxGroupTest)
{
    const char text[] = " My mailbox name: my.test@test.example.com, my.test.2@test.example.com ";
    MU_MailboxGroup * mailbox_group = muMailboxGroupParse(text);
    BOOST_CHECK(mailbox_group);
    BOOST_CHECK_EQUAL("My mailbox name", muMailboxGroupName(mailbox_group));
    BOOST_CHECK_EQUAL(2, muMailboxCount(mailbox_group));

    MU_Mailbox * mailbox = muMailbox(mailbox_group, 0);
    BOOST_CHECK(mailbox);
    BOOST_CHECK(nullptr == muMailboxName(mailbox));
    BOOST_CHECK_EQUAL("my.test@test.example.com", muMailboxAddress(mailbox));
    muFree(mailbox);

    mailbox = muMailbox(mailbox_group, 1);
    BOOST_CHECK(mailbox);
    BOOST_CHECK(nullptr == muMailboxName(mailbox));
    BOOST_CHECK_EQUAL("my.test.2@test.example.com", muMailboxAddress(mailbox));
    muFree(mailbox);
    muFree(mailbox_group);
}

BOOST_AUTO_TEST_CASE(ParseNamedMailboxGroupWithNamedMailboxesTest)
{
    const char text[] = " My mailbox name: Mailbox 1 <my.test@test.example.com>, "
            "Mailbox 2 <my.test.2@test.example.com> ";
    MU_MailboxGroup * mailbox_group = muMailboxGroupParse(text);
    BOOST_CHECK(mailbox_group);
    BOOST_CHECK_EQUAL("My mailbox name", muMailboxGroupName(mailbox_group));
    BOOST_CHECK_EQUAL(2, muMailboxCount(mailbox_group));

    MU_Mailbox * mailbox = muMailbox(mailbox_group, 0);
    BOOST_CHECK(mailbox);
    BOOST_CHECK_EQUAL("Mailbox 1", muMailboxName(mailbox));
    BOOST_CHECK_EQUAL("my.test@test.example.com", muMailboxAddress(mailbox));
    muFree(mailbox);

    mailbox = muMailbox(mailbox_group, 1);
    BOOST_CHECK(mailbox);
    BOOST_CHECK_EQUAL("Mailbox 2", muMailboxName(mailbox));
    BOOST_CHECK_EQUAL("my.test.2@test.example.com", muMailboxAddress(mailbox));
    muFree(mailbox);
    muFree(mailbox_group);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace LibMailUnit
} // namespace Test
