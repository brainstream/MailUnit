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

#ifndef __MU_OS_FILESYSTEM_H__
#define __MU_OS_FILESYSTEM_H__

#include <cstdint>
#include <utility>
#include <boost/filesystem.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/locale/encoding.hpp>
#include <LibMailUnit/Api/Include/Def.h>

#ifdef BOOST_WINDOWS_API
#   define MU_PATHISWIDECHAR
#   define MU_PATHSTR(str) L ## str
#else
#   define MU_PATHSTR(str) str
#endif

namespace MailUnit {
namespace OS {

static const uint16_t file_open_read   = 0x01;
static const uint16_t file_open_write  = 0x02;
static const uint16_t file_open_create = 0x04;
static const uint16_t file_open_append = 0x08;

typedef boost::filesystem::path::string_type PathString;

void closeNativeFile(MU_File _native_file);

inline void deleteFile(const PathString & _filepath)
{
    boost::filesystem::remove(_filepath);
}

inline void deleteFile(const boost::filesystem::path & _filepath)
{
    boost::filesystem::remove(_filepath);
}

inline boost::filesystem::path tempFilepath()
{
    return boost::filesystem::temp_directory_path() / boost::filesystem::unique_path();
}

inline PathString utf8ToPathString(const std::string & _mb_string)
{
#ifdef MU_PATHISWIDECHAR
    return boost::locale::conv::utf_to_utf<wchar_t>(_mb_string);
#else
    return _mb_string;
#endif
}

boost::filesystem::path systemConfigDirectory();

boost::filesystem::path userConfigDirectory();

boost::filesystem::path toAbsolutePath(const boost::filesystem::path & _path,
    const boost::filesystem::path & _app_path);

class File
{
public:
    File(const boost::filesystem::path & _filepath, uint16_t _flags, bool _autoclose = true) :
        File(openNativeFile(_filepath, _flags), _autoclose)
    {
    }

    File(MU_File _native_file, bool _autoclose) :
        m_file(boost::iostreams::file_descriptor(_native_file, s_descriptor_flags)),
        m_autoclose(_autoclose)
    {
    }

    File(const File &) = delete;

    File(File && _native_file) :
        m_file(boost::iostreams::file_descriptor(_native_file.reset(), s_descriptor_flags))
    {
    }

    File & operator = (const File &) = delete;

    File & operator = (File && _native_file)
    {
        if(this == &_native_file)
            return * this;
        if(m_file.handle() != _native_file.m_file.handle())
            closeNativeFile(m_file.handle());
        m_file = boost::iostreams::file_descriptor(_native_file.reset(), s_descriptor_flags);
        return *this;
    }

    virtual ~File()
    {
        if(m_autoclose)
            closeNativeFile(m_file.handle());
    }

    operator MU_File ()
    {
        return m_file.handle();
    }

    MU_File native()
    {
        return m_file.handle();
    }

    boost::iostreams::file_descriptor & getDescriptor()
    {
        return m_file;
    }

    bool isOpen() const
    {
        return m_file.is_open();
    }

    std::streamsize read(char * _buffer, std::streamsize _length)
    {
        return m_file.read(_buffer, _length);
    }

    std::streamsize write(const std::string & _str)
    {
        return m_file.write(_str.c_str(), _str.size());
    }

    std::streamsize write(const char * _buffer, std::streamsize _length)
    {
        return m_file.write(_buffer, _length);
    }

    std::streampos seek(std::streamoff _offset, std::ios_base::seekdir _seekdir)
    {
        return m_file.seek(_offset, _seekdir);
    }

    MU_File reset()
    {
        return swap(MU_INVALID_FILE);
    }

    MU_File swap(MU_File _new_native_file)
    {
        MU_File old_file = m_file.handle();
        m_file.open(_new_native_file, s_descriptor_flags);
        return old_file;
    }

protected:
    static MU_File openNativeFile(const boost::filesystem::path & _filepath, uint16_t _nf_flags);

private:
    static const boost::iostreams::file_descriptor_flags s_descriptor_flags =
        boost::iostreams::never_close_handle;
    boost::iostreams::file_descriptor m_file;
    bool m_autoclose;
}; // class File

class TempFile : public File
{
public:
    TempFile() :
        TempFile(init())
    {
    }

    ~TempFile() override
    {
        MU_File file = reset();
        closeNativeFile(file);
        boost::filesystem::remove(m_filepath);
    }

    const boost::filesystem::path & path() const
    {
        return m_filepath;
    }

private:
    explicit TempFile(std::pair<MU_File, boost::filesystem::path> && _init_data) :
        File(_init_data.first, false),
        m_filepath(_init_data.second)
    {
    }

    static std::pair<MU_File, boost::filesystem::path> init()
    {
        boost::filesystem::path path = tempFilepath();;
        MU_File file = openNativeFile(path, file_open_create | file_open_read | file_open_write);
        return std::make_pair(file, path);
    }

private:
    boost::filesystem::path m_filepath;
}; // class TempFile

} // namespace OS
} // namespace MailUnit


#endif // __MU_OS_FILESYSTEM_H__
