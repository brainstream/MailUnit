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

// FIXME: uncomment

//#include <LibMailUnit/Memory.h>

namespace LibMailUnit {
namespace Test {

BOOST_AUTO_TEST_SUITE(Memory)

//struct Test
//{
//    Test(bool * _dtor_flag) :
//        Test(0, std::string(), _dtor_flag)
//    {
//    }

//    Test(int _num, const std::string & _str, bool * _dtor_flag) :
//        num(_num),
//        str(_str),
//        dtor_flag(_dtor_flag)
//    {
//    }

//    ~Test()
//    {
//        *dtor_flag = true;
//    }

//    int num;
//    std::string str;
//    bool * dtor_flag;
//};

BOOST_AUTO_TEST_CASE(handleTest)
{
//    bool destructor_called = false;
//    Test * test = new Test(&destructor_called);
//    MU_HANDLE handle = new ApiObject(test, false);
//    BOOST_CHECK_EQUAL(test, handle->pointer<Test>());
//    muFree(handle);
//    BOOST_CHECK_EQUAL(false, destructor_called);
//    handle = new ApiObject(test, true);
//    muFree(handle);
//    BOOST_CHECK_EQUAL(true, destructor_called);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace Test
} // namespace LibMailUnit
