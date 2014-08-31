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
#include <MailUnit/Remote/Edsl.h>

using namespace MailUnit::Remote::Edsl;

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
            "select * fom Test;"
        },
        {
            false,
            "select from Test;"
        },
        {
            false,
            "select * from;"
        },
        {
            false,
            "celect * from Test;"
        },
        {
            true,
            "select * from Test;",
            "SELECT * FROM Test;"
        },
        {
            true,
            "select * from Test where Number = 123;",
            "SELECT * FROM Test WHERE (Number = 123);"
        },
        {
            true,
            "select * from Test where Name = 'SomeValue';",
            "SELECT * FROM Test WHERE (Name = 'SomeValue');"
        },
        {
            true,
            "select * from Test where Name = 'Some Value';",
            "SELECT * FROM Test WHERE (Name = 'Some Value');"
        },
        {
            true,
            "select * from Test where Number = 123 AND Name <> 'Some Value';",
            "SELECT * FROM Test WHERE (Number = 123 AND Name <> 'Some Value');"
        },
        {
            true,
            "select * from Test where Number = 123 or Name = 'Some Value';",
            "SELECT * FROM Test WHERE (Number = 123 OR Name = 'Some Value');"
        },
        {
            true,
            "select * from Test where (Number = 123 and Name = 'Some Value') or Surname = 'Other';",
            "SELECT * FROM Test WHERE ((Number = 123 AND Name = 'Some Value') OR Surname = 'Other');"
        },
        {
            true,
            "select * from Test where Surname = 'Other' or (Number > 123 and Name = 'Some Value');",
            "SELECT * FROM Test WHERE (Surname = 'Other' OR (Number > 123 AND Name = 'Some Value'));"
        },
        {
            true,
            "select * from Test where (Age >= 12 and Surname = 'Other') or (Number < 123 and Name = 'Some Value');",
            "SELECT * FROM Test WHERE ((Age >= 12 AND Surname = 'Other') OR (Number < 123 AND Name = 'Some Value'));"
        },
        {
            true,
            "select * from Test where (Age >= 12 and Surname = 'Other') or (Number = 123 and Name = 'Some Value') or !Bool;",
            "SELECT * FROM Test WHERE ((Age >= 12 AND Surname = 'Other') OR (Number = 123 AND Name = 'Some Value') OR !Bool);"
        },
        {
            true,
            "select * from Test where (Age >= 12 and Age <= 100 and Surname = 'Other') or (Number = 123 and Name = 'Some Value') or !Bool;",
            "SELECT * FROM Test WHERE ((Age >= 12 AND Age <= 100 AND Surname = 'Other') OR (Number = 123 AND Name = 'Some Value') OR !Bool);"
        },
        {
            true,
            "select * from Test where ((Age >= 12 and Surname = 'Other') or !Bool) or (Number = 123 and Name = 'Some Value');",
            "SELECT * FROM Test WHERE (((Age >= 12 AND Surname = 'Other') OR !Bool) OR (Number = 123 AND Name = 'Some Value'));"
        },
        {
            true,
            "select One from Test;",
            "SELECT One FROM Test;"
        },
        {
            true,
            "select One, Two, Three from Test;",
            "SELECT One, Two, Three FROM Test;"
        },
        {
            true,
            "select One,Two,Three from Test;",
            "SELECT One, Two, Three FROM Test;"
        }
    };
    for(auto test : tests)
    {
        std::shared_ptr<Expression> expression = parse(test.query);
        if(test.valid)
        {
            BOOST_CHECK(nullptr != expression);
            std::stringstream stream;
            stream << *expression;
            BOOST_CHECK_EQUAL(test.expected_result, stream.str());
        }
        else
        {
            BOOST_CHECK(nullptr == expression);
            continue;
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace Test
} // namespace MailUnit
