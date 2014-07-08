#include <boost/test/unit_test.hpp>
#include <LibMailUnit/InternalMemory.h>

BOOST_AUTO_TEST_SUITE(Memory)

BOOST_AUTO_TEST_CASE(makeObjectHandleTest)
{
    struct Test
    {
    public:
        Test(int _num, const std::string & _str, bool * _dtor_flag) :
            num(_num),
            str(_str),
            dtor_flag(_dtor_flag)
        {
        }

        ~Test()
        {
            *dtor_flag = true;
        }

        int num;
        std::string str;
        bool * dtor_flag;
    };
    bool destructor_called = false;
    MU_HANDLE handle = LibMailUnit::makeObjectHandle<Test>(10, "test", &destructor_called);
    Test * test = LibMailUnit::handlePointer<Test>(handle);
    BOOST_CHECK_EQUAL(10, test->num);
    BOOST_CHECK_EQUAL(std::string("test"), test->str);
    BOOST_CHECK(!destructor_called);
    muFree(handle);
    BOOST_CHECK(destructor_called);
}

BOOST_AUTO_TEST_SUITE_END()
