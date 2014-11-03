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

#include <boost/test/unit_test.hpp>
#include <MailUnit/DeferredPointer.h>

using namespace MailUnit;

namespace MailUnit {
namespace Test {

namespace {

bool constructed = false;

struct Test
{
    explicit Test(int _x) : x(_x)
    {
        constructed = true;
    }

    ~Test()
    {
        constructed = false;
    }

    int x;
}; // struct Test

} // namespace

BOOST_AUTO_TEST_SUITE(DeferredPointer)

BOOST_AUTO_TEST_CASE(destructTest)
{
    constructed = false;
    MailUnit::DeferredPointer<Test> dpointer;
    BOOST_CHECK(!dpointer.constructed());
    dpointer.construct(100);
    BOOST_CHECK(constructed);
    BOOST_CHECK(dpointer.constructed());
    dpointer.destruct();
    BOOST_CHECK(!constructed);
    BOOST_CHECK(!dpointer.constructed());
}

BOOST_AUTO_TEST_CASE(autodestructTest)
{
    constructed = false;
    {
        MailUnit::DeferredPointer<Test> dpointer;
        dpointer.construct(100);
        BOOST_CHECK(constructed);
    }
    BOOST_CHECK(!constructed);
}

BOOST_AUTO_TEST_CASE(operatorsTest)
{
    MailUnit::DeferredPointer<Test> dpointer;
    Test * clone = dpointer.unsafeGet();
    Test not_clone(200);
    BOOST_CHECK(!dpointer);
    BOOST_CHECK(dpointer == nullptr);
    BOOST_CHECK(dpointer != clone);
    BOOST_CHECK(dpointer != &not_clone);
    BOOST_CHECK(dpointer.get() == nullptr);
    BOOST_CHECK(dpointer.unsafeGet() == clone);
    dpointer.construct(100);
    BOOST_CHECK(dpointer == true);
    BOOST_CHECK(dpointer != nullptr);
    BOOST_CHECK(dpointer == clone);
    BOOST_CHECK(dpointer != &not_clone);
    BOOST_CHECK(dpointer.get() == clone);
    BOOST_CHECK(dpointer.unsafeGet() == clone);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace Test
} // namespace MailUnit
