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

#ifndef __LIBMUTESTS_TESTFIXTURE_H__
#define __LIBMUTESTS_TESTFIXTURE_H__

#include <map>
#include <LibMailUnit/Def.h>
#ifdef _WIN32
#   include <windows.h>
#else
#   include <cstdlib>
#   include <unistd.h>
#endif


namespace LibMailUnit {
namespace Test {

class TestFixture
{
public:
    ~TestFixture();
    MU_NATIVE_FILE createTempFile(const std::string & _content);
    void releaseFile(MU_NATIVE_FILE _file);

private:
    void deleteFile(const std::pair<MU_NATIVE_FILE, std::string> & _file);

private:
    std::map<MU_NATIVE_FILE, std::string> m_files;
}; // class TestFixture


} // namespace Test
} // namespace LibMailUnit

#endif // __LIBMUTESTS_TESTFIXTURE_H__
