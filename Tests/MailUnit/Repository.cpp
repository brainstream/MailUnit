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
#include <MailUnit/Storage/Repository.h>

using namespace MailUnit::Storage;

namespace {

struct TestContext
{
    TestContext()
    {
        repository_path = MailUnit::OS::tempFilepath();
    }

    ~TestContext()
    {
        if(boost::filesystem::is_directory(repository_path))
        {
            boost::filesystem::remove_all(repository_path);
        }
    }

    boost::filesystem::path repository_path;
}; // struct TestContext

} // namespace

namespace MailUnit {
namespace Test {

BOOST_AUTO_TEST_SUITE(RepositoryTests)

BOOST_AUTO_TEST_CASE(createRawEmailTest)
{
    TestContext context;
    Repository repository(context.repository_path);
    std::unique_ptr<RawEmail> raw_email = repository.createRawEmail();
    BOOST_CHECK(boost::filesystem::is_regular_file(raw_email->dataFilePath()));
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace Test
} // namespace MailUnit
