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
#include <MailUnit/OS/FileSystem.h>

using namespace MailUnit::OS;

namespace MailUnit {
namespace Test {

namespace {

static const std::string test_content = "123456789";

} // namespace

BOOST_AUTO_TEST_SUITE(File)

BOOST_AUTO_TEST_CASE(openNewReadonlyFileTest)
{
    boost::filesystem::path path = tempFilepath();
    OS::File file(path, nf_open_create);
    BOOST_CHECK_EQUAL(MU_INVALID_NATIVE_FILE, file.native());
    BOOST_CHECK(boost::filesystem::exists(path) == false);
}

BOOST_AUTO_TEST_CASE(openNewWriteOnlyFileTest)
{
    boost::filesystem::path path = tempFilepath();
    OS::File file(path, nf_open_create | nf_open_write);
    BOOST_CHECK_NE(MU_INVALID_NATIVE_FILE, file.native());
    BOOST_CHECK(boost::filesystem::exists(path) == true);
    boost::filesystem::perms perms = boost::filesystem::status(path).permissions();
    BOOST_CHECK_EQUAL(boost::filesystem::owner_write | boost::filesystem::owner_read, perms);
    closeNativeFile(file);
    file.reset();
    BOOST_CHECK_EQUAL(MU_INVALID_NATIVE_FILE, file.native());
    deleteFile(path);
    BOOST_CHECK(boost::filesystem::exists(path) == false);
}

BOOST_AUTO_TEST_CASE(openNewReadWriteOnlyFileTest)
{
    boost::filesystem::path path = tempFilepath();
    OS::File file(path, nf_open_create | nf_open_write | nf_open_read);
    BOOST_CHECK_NE(MU_INVALID_NATIVE_FILE, file.native());
    BOOST_CHECK(boost::filesystem::exists(path) == true);
    boost::filesystem::perms perms = boost::filesystem::status(path).permissions();
    BOOST_CHECK_EQUAL(boost::filesystem::owner_write | boost::filesystem::owner_read, perms);
    closeNativeFile(file);
    file.reset();
    BOOST_CHECK_EQUAL(MU_INVALID_NATIVE_FILE, file.native());
    deleteFile(path);
    BOOST_CHECK(boost::filesystem::exists(path) == false);
}

boost::filesystem::path createAndWriteTestConten()
{
    boost::filesystem::path path = tempFilepath();
    {
        OS::File file(path, nf_open_create | nf_open_write);
        file.write(test_content);
    }
    size_t size = boost::filesystem::file_size(path);
    if(test_content.size() != size)
    {
        throw std::runtime_error("The size of the file is not equals to the size of the test content");
    }
    return path;
}

BOOST_AUTO_TEST_CASE(openExistentReadOnlyFileTest)
{
    boost::filesystem::path path = createAndWriteTestConten();
    OS::File file(path, nf_open_read);
    BOOST_CHECK_NE(MU_INVALID_NATIVE_FILE, file.native());
    BOOST_CHECK(boost::filesystem::exists(path) == true);
    closeNativeFile(file);
    file.reset();
    BOOST_CHECK_EQUAL(MU_INVALID_NATIVE_FILE, file.native());
    size_t size = boost::filesystem::file_size(path);
    BOOST_CHECK_EQUAL(test_content.size(), size);
    deleteFile(path);
    BOOST_CHECK(boost::filesystem::exists(path) == false);
}

BOOST_AUTO_TEST_CASE(openExistentAppendFileTest)
{
    boost::filesystem::path path = createAndWriteTestConten();
    OS::File file(path, nf_open_write | nf_open_append);
    BOOST_CHECK_NE(MU_INVALID_NATIVE_FILE, file.native());
    BOOST_CHECK(boost::filesystem::exists(path) == true);
    size_t size = boost::filesystem::file_size(path);
    BOOST_CHECK_EQUAL(test_content.size(), size);
    file.write("123");
    size = boost::filesystem::file_size(path);
    BOOST_CHECK_EQUAL(test_content.size() + 3, size);
    closeNativeFile(file);
    file.reset();
    BOOST_CHECK_EQUAL(MU_INVALID_NATIVE_FILE, file.native());
    deleteFile(path);
    BOOST_CHECK(boost::filesystem::exists(path) == false);
}

BOOST_AUTO_TEST_CASE(openExistentTruncateFileTest)
{
    boost::filesystem::path path = createAndWriteTestConten();
    OS::File file(path, nf_open_write | nf_open_create);
    BOOST_CHECK_NE(MU_INVALID_NATIVE_FILE, file.native());
    BOOST_CHECK(boost::filesystem::exists(path) == true);
    size_t size = boost::filesystem::file_size(path);
    BOOST_CHECK_EQUAL(0, size);
    file.write("123");
    size = boost::filesystem::file_size(path);
    BOOST_CHECK_EQUAL(3, size);
    closeNativeFile(file);
    file.reset();
    BOOST_CHECK_EQUAL(MU_INVALID_NATIVE_FILE, file.native());
    deleteFile(path);
    BOOST_CHECK(boost::filesystem::exists(path) == false);
}

BOOST_AUTO_TEST_CASE(tempFileTest)
{
    boost::filesystem::path path;
    {
        TempFile temp_file;
        path = temp_file.path();
        BOOST_CHECK_NE(MU_INVALID_NATIVE_FILE, temp_file.native());
        BOOST_CHECK(boost::filesystem::exists(path) == true);
        boost::filesystem::perms perms = boost::filesystem::status(path).permissions();
        BOOST_CHECK_EQUAL(boost::filesystem::owner_write | boost::filesystem::owner_read, perms);
    }
    BOOST_CHECK(boost::filesystem::exists(path) == false);
}


BOOST_AUTO_TEST_SUITE_END()

} // namespace Test
} // namespace MailUnit
