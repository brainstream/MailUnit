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
            "invalid text",
        },
        {
            true,
            "get \t\v\b\r\n",
            "GET;"
        },
        {
            false,
            "get Number",
        },
        {
            false,
            "get Number =",
        },
        {
            false,
            "get Number = X",
        },
        {
            false,
            "get (Number = 123;"
        },
        {
            false,
            "get Name = 'BrokenString;"
        },
        {
            true,
            "get Number = 123",
            "GET (Number = 123);"
        },
        {
            true,
            "gEt Name = 'SomeValue'",
            "GET (Name = 'SomeValue');"
        },
        {
            true,
            "dRoP Name = 'Some Value'",
            "DROP (Name = 'Some Value');"
        },
        {
            true,
            "drop Number = 123 AND Name <> 'Some Value'",
            "DROP (Number = 123 AND Name <> 'Some Value');"
        },
        {
            true,
            "get Number = 123 or Name = 'Some Value'",
            "GET (Number = 123 OR Name = 'Some Value');"
        },
        {
            true,
            "get (Number = 123 and Name = 'Some Value') or Surname = 'Other'",
            "GET ((Number = 123 AND Name = 'Some Value') OR Surname = 'Other');"
        },
        {
            true,
            "drop Surname = 'Other' or (Number > 123 and Name = 'Some Value')",
            "DROP (Surname = 'Other' OR (Number > 123 AND Name = 'Some Value'));"
        },
        {
            true,
            "get (Age >= 12 and Surname = 'Other') or (Number < 123 and Name = 'Some Value')",
            "GET ((Age >= 12 AND Surname = 'Other') OR (Number < 123 AND Name = 'Some Value'));"
        },
        {
            false,
            "get (Age >= 12 and Surname = 'Other') or (Number < 123 and Name = ValueWithoutQuotes)"
        },
        {
            true,
            "get Subject = 'test' and (From = 'from@test' or To = 'to@test')",
            "GET (Subject = 'test' AND (From = 'from@test' OR To = 'to@test'));"
        }
    };
    for(auto test : tests)
    {
        try
        {
            std::shared_ptr<Expression> expression = parse(test.query);
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
