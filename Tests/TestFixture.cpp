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

#include "TestFixture.h"

using namespace LibMailUnit::Test;

TestFixture::~TestFixture()
{
    for(auto & file: m_files)
        deleteFile(file);
}

MU_NATIVE_FILE TestFixture::createTempFile(const std::string & _content)
{
#ifdef _WIN32
#else
    char tmpl[] = "/tmp/XXXXXX";
    int file = mkstemp(tmpl);
    m_files.insert({ file, std::string(tmpl) });
    write(file, _content.c_str(), _content.size());
    lseek(file, 0, SEEK_SET);
    return file;
#endif
}

void TestFixture::releaseFile(MU_NATIVE_FILE _file)
{
    auto it = m_files.find(_file);
    if(m_files.end() != it)
    {
        deleteFile(*it);
        m_files.erase(it);
    }
}

void TestFixture::deleteFile(const std::pair<MU_NATIVE_FILE, std::string> & _file)
{
#ifdef _WIN32
#else
        close(_file.first);
        unlink(_file.second.c_str());
#endif
}
