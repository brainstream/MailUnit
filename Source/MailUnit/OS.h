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

#ifndef __MU_OS_H__
#define __MU_OS_H__

#include <LibMailUnit/Def.h>
#include <boost/noncopyable.hpp>
#include <boost/filesystem/path.hpp>

namespace MailUnit {

#ifdef BOOST_WINDOWS_API
#   define MU_PATHISWIDECHAR
#   define MU_PATHSTR(str) L ## str;
#else
#   define MU_PATHSTR(str) str
#endif

typedef typename boost::filesystem::path::string_type PathString;

constexpr int open_nf_read   = 0x01;
constexpr int open_nf_write  = 0x02;
constexpr int open_nf_create = 0x04;
constexpr int open_nf_append = 0x08;

MU_NATIVE_FILE openNativeFile(const boost::filesystem::path & _filepath, int _flags);
void closeNativeFile(MU_NATIVE_FILE _file);

class NativeFile final : private boost::noncopyable
{
public:
    NativeFile(const boost::filesystem::path & _filepath, int _flags) :
        m_file(openNativeFile(_filepath, _flags))
    {
    }

    ~NativeFile()
    {
        closeNativeFile(m_file);
    }

    operator MU_NATIVE_FILE () const
    {
        return m_file;
    }

    bool isOpen() const
    {
        return MU_INVALID_NATIVE_FILE != m_file;
    }

private:
    MU_NATIVE_FILE m_file;
}; // class NativeFile

} // namespace MailUnit

#endif // __MU_OS_H__
