#include <boost/test/unit_test.hpp>
#include <LibMailUnit/InternalMemory.h>

BOOST_AUTO_TEST_SUITE(Memory)

struct Test
{
    Test(bool * _dtor_flag) :
        Test(0, std::string(), _dtor_flag)
    {
    }

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

BOOST_AUTO_TEST_CASE(muAllocTest)
{
    static bool destructor_called = false;
    MU_HANDLE handle = muAlloc(8, [](void *) {
        destructor_called = true;
    });
    BOOST_CHECK(!destructor_called);
    muFree(handle);
    BOOST_CHECK(destructor_called);
}

BOOST_AUTO_TEST_CASE(makeObjectHandleTest)
{
    bool destructor_called = false;
    MU_HANDLE handle = LibMailUnit::makeObjectHandle<Test>(10, "test", &destructor_called);
    Test * test = LibMailUnit::handlePointer<Test>(handle);
    BOOST_CHECK_EQUAL(10, test->num);
    BOOST_CHECK_EQUAL(std::string("test"), test->str);
    BOOST_CHECK(!destructor_called);
    muFree(handle);
    BOOST_CHECK(destructor_called);
}

BOOST_AUTO_TEST_CASE(wrapPointerTest)
{
    bool destructor_called = false;
    Test * test = new Test(&destructor_called);
    MU_HANDLE handle = LibMailUnit::wrapPointer(test);
    BOOST_CHECK_EQUAL(test, LibMailUnit::handlePointer<Test>(handle));
    muFree(handle);
    BOOST_CHECK(!destructor_called);
    delete test;
}

BOOST_AUTO_TEST_SUITE_END()
