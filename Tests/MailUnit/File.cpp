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
    OS::File file(path, file_open_create, false);
    BOOST_CHECK_EQUAL(MU_INVALID_FILE, file.native());
    BOOST_CHECK(boost::filesystem::exists(path) == false);
}

BOOST_AUTO_TEST_CASE(openNewWriteOnlyFileTest)
{
    boost::filesystem::path path = tempFilepath();
    OS::File file(path, file_open_create | file_open_write, false);
    BOOST_CHECK_NE(MU_INVALID_FILE, file.native());
    BOOST_CHECK(boost::filesystem::exists(path) == true);
    boost::filesystem::perms perms = boost::filesystem::status(path).permissions();
    BOOST_CHECK_EQUAL(boost::filesystem::owner_write | boost::filesystem::owner_read, perms);
    closeNativeFile(file);
    file.reset();
    BOOST_CHECK_EQUAL(MU_INVALID_FILE, file.native());
    deleteFile(path);
    BOOST_CHECK(boost::filesystem::exists(path) == false);
}

BOOST_AUTO_TEST_CASE(openNewReadWriteOnlyFileTest)
{
    boost::filesystem::path path = tempFilepath();
    OS::File file(path, file_open_create | file_open_write | file_open_read, false);
    BOOST_CHECK_NE(MU_INVALID_FILE, file.native());
    BOOST_CHECK(boost::filesystem::exists(path) == true);
    boost::filesystem::perms perms = boost::filesystem::status(path).permissions();
    BOOST_CHECK_EQUAL(boost::filesystem::owner_write | boost::filesystem::owner_read, perms);
    closeNativeFile(file);
    file.reset();
    BOOST_CHECK_EQUAL(MU_INVALID_FILE, file.native());
    deleteFile(path);
    BOOST_CHECK(boost::filesystem::exists(path) == false);
}

boost::filesystem::path createAndWriteTestConten()
{
    boost::filesystem::path path = tempFilepath();
    {
        OS::File file(path, file_open_create | file_open_write);
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
    OS::File file(path, file_open_read, false);
    BOOST_CHECK_NE(MU_INVALID_FILE, file.native());
    BOOST_CHECK(boost::filesystem::exists(path) == true);
    closeNativeFile(file);
    file.reset();
    BOOST_CHECK_EQUAL(MU_INVALID_FILE, file.native());
    size_t size = boost::filesystem::file_size(path);
    BOOST_CHECK_EQUAL(test_content.size(), size);
    deleteFile(path);
    BOOST_CHECK(boost::filesystem::exists(path) == false);
}

BOOST_AUTO_TEST_CASE(openExistentAppendFileTest)
{
    boost::filesystem::path path = createAndWriteTestConten();
    OS::File file(path, file_open_write | file_open_append, false);
    BOOST_CHECK_NE(MU_INVALID_FILE, file.native());
    BOOST_CHECK(boost::filesystem::exists(path) == true);
    size_t size = boost::filesystem::file_size(path);
    BOOST_CHECK_EQUAL(test_content.size(), size);
    file.write("123");
    size = boost::filesystem::file_size(path);
    BOOST_CHECK_EQUAL(test_content.size() + 3, size);
    closeNativeFile(file);
    file.reset();
    BOOST_CHECK_EQUAL(MU_INVALID_FILE, file.native());
    deleteFile(path);
    BOOST_CHECK(boost::filesystem::exists(path) == false);
}

BOOST_AUTO_TEST_CASE(openExistentTruncateFileTest)
{
    boost::filesystem::path path = createAndWriteTestConten();
    OS::File file(path, file_open_write | file_open_create, false);
    BOOST_CHECK_NE(MU_INVALID_FILE, file.native());
    BOOST_CHECK(boost::filesystem::exists(path) == true);
    size_t size = boost::filesystem::file_size(path);
    BOOST_CHECK_EQUAL(0, size);
    file.write("123");
    size = boost::filesystem::file_size(path);
    BOOST_CHECK_EQUAL(3, size);
    closeNativeFile(file);
    file.reset();
    BOOST_CHECK_EQUAL(MU_INVALID_FILE, file.native());
    deleteFile(path);
    BOOST_CHECK(boost::filesystem::exists(path) == false);
}

BOOST_AUTO_TEST_CASE(autocloseTrueTest)
{
    MU_File native_file = MU_INVALID_FILE;
    boost::filesystem::path path = tempFilepath();
    {
        OS::File file(path, file_open_create | file_open_write | file_open_read, true);
        native_file = file.native();
        boost::iostreams::file_descriptor fd(native_file, boost::iostreams::never_close_handle);
        fd.write(test_content.c_str(), test_content.size());
        size_t size = boost::filesystem::file_size(path);
        BOOST_CHECK_EQUAL(test_content.size(), size);
    }
    boost::iostreams::file_descriptor fd(native_file, boost::iostreams::never_close_handle);
    BOOST_CHECK_THROW(fd.write(test_content.c_str(), test_content.size()), std::exception);
    deleteFile(path);
}

BOOST_AUTO_TEST_CASE(autocloseFalseTest)
{
    MU_File native_file = MU_INVALID_FILE;
    boost::filesystem::path path = tempFilepath();
    {
        OS::File file(path, file_open_create | file_open_write | file_open_read, false);
        native_file = file.native();
        boost::iostreams::file_descriptor fd(native_file, boost::iostreams::never_close_handle);
        fd.write(test_content.c_str(), test_content.size());
        size_t size = boost::filesystem::file_size(path);
        BOOST_CHECK_EQUAL(test_content.size(), size);
    }
    {
        boost::iostreams::file_descriptor fd(native_file, boost::iostreams::never_close_handle);
        BOOST_CHECK_NO_THROW(fd.write(test_content.c_str(), test_content.size()));
        size_t size = boost::filesystem::file_size(path);
        BOOST_CHECK_EQUAL(test_content.size() * 2, size);
        closeNativeFile(native_file);
    }
    {
        boost::iostreams::file_descriptor fd(native_file, boost::iostreams::never_close_handle);
        BOOST_CHECK_THROW(fd.write(test_content.c_str(), test_content.size()), std::exception);
    }
    deleteFile(path);
}

BOOST_AUTO_TEST_CASE(tempFileTest)
{
    boost::filesystem::path path;
    {
        TempFile temp_file;
        path = temp_file.path();
        BOOST_CHECK_NE(MU_INVALID_FILE, temp_file.native());
        BOOST_CHECK(boost::filesystem::exists(path) == true);
        boost::filesystem::perms perms = boost::filesystem::status(path).permissions();
        BOOST_CHECK_EQUAL(boost::filesystem::owner_write | boost::filesystem::owner_read, perms);
    }
    BOOST_CHECK(boost::filesystem::exists(path) == false);
}


BOOST_AUTO_TEST_SUITE_END()

} // namespace Test
} // namespace MailUnit
