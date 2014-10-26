/***********************************************************************************************
 *                                                                                             *
 * This file is part of MailUnit.                                                              *
 *                                                                                             *
 * MailUnit is free software: you can redistribute it and/or modify it under the terms of      *
 * the GNU General Public License as published by the Free Software Foundation,                *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * MailUnit is distributed in the hope that it will be useful,  but WITHOUT ANY WARRANTY;      *
 * without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  *
 * See the GNU General Public License for more details.                                        *
 *                                                                                             *
 * You should have received a copy of the GNU General Public License along with MailUnit.      *
 * If not, see <http://www.gnu.org/licenses/>.                                                 *
 *                                                                                             *
 ***********************************************************************************************/

#include <sstream>
#include <boost/test/unit_test.hpp>
#include <MailUnit/Storage/Edsl.h>

using namespace MailUnit::Storage::Edsl;

namespace MailUnit {
namespace Test {

struct EdslTestQuery
{
    bool valid;
    std::string query;
    std::string expected_result;
}; // struct EdslTestQuery

BOOST_AUTO_TEST_SUITE(Edsl)

BOOST_AUTO_TEST_CASE(parseTest)
{
    const EdslTestQuery tests[] = {
        {
            false,
            "",
        },
        {
            false,
            " \t\v\b\r\n",
        },
        {
            false,
            "Number",
        },
        {
            false,
            "Number =",
        },
        {
            false,
            "Number = X",
        },
        {
            false,
            "(Number = 123"
        },
        {
            false,
            "Name = 'BrokenString"
        },
        {
            true,
            "Number = 123",
            "(Number = 123)"
        },
        {
            true,
            "Name = 'SomeValue'",
            "(Name = 'SomeValue')"
        },
        {
            true,
            "Name = 'Some Value'",
            "(Name = 'Some Value')"
        },
        {
            true,
            "Number = 123 AND Name <> 'Some Value'",
            "(Number = 123 AND Name <> 'Some Value')"
        },
        {
            true,
            "Number = 123 or Name = 'Some Value'",
            "(Number = 123 OR Name = 'Some Value')"
        },
        {
            true,
            "(Number = 123 and Name = 'Some Value') or Surname = 'Other'",
            "((Number = 123 AND Name = 'Some Value') OR Surname = 'Other')"
        },
        {
            true,
            "Surname = 'Other' or (Number > 123 and Name = 'Some Value')",
            "(Surname = 'Other' OR (Number > 123 AND Name = 'Some Value'))"
        },
        {
            true,
            "(Age >= 12 and Surname = 'Other') or (Number < 123 and Name = 'Some Value')",
            "((Age >= 12 AND Surname = 'Other') OR (Number < 123 AND Name = 'Some Value'))"
        },
        {
            false,
            "(Age >= 12 and Surname = 'Other') or (Number < 123 and Name = ValueWithoutQuotes)"
        },
        {
            true,
            "Subject = 'test' and (From = 'from@test' or To = 'to@test')",
            "(Subject = 'test' AND (From = 'from@test' OR To = 'to@test'))"
        }
    };
    for(auto test : tests)
    {
        try
        {
            std::shared_ptr<ConditionSequence> expression = parse(test.query);
            BOOST_CHECK(test.valid);
            BOOST_CHECK(nullptr != expression);
            std::stringstream stream;
            stream << *expression;
            BOOST_CHECK_EQUAL(test.expected_result, stream.str());
        }
        catch(const EdslException &)
        {
            BOOST_CHECK(!test.valid);
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace Test
} // namespace MailUnit
