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

#include <MailUnit/OS/FileSystem.h>
#ifdef BOOST_WINDOWS_API
#   include <Windows.h>
#else
#   include <unistd.h>
#   include <fcntl.h>
#endif

using namespace MailUnit::OS;
namespace fs = boost::filesystem;

MU_NATIVE_FILE File::openNativeFile(const fs::path & _filepath, uint16_t _nf_flags)
{
#ifdef _WIN32
    int desired_access = 0;
    int share_nf_flags = FILE_SHARE_READ;
    int create_flag = OPEN_EXISTING;
    if(_nf_flags & nf_open_read && _nf_flags & nf_open_write)
        desired_access = GENERIC_ALL;
    else if(_nf_flags & nf_open_read)
        desired_access = GENERIC_READ;
    else if(_nf_flags & nf_open_write)
        desired_access = GENERIC_WRITE;
    else
        return INVALID_HANDLE_VALUE;
    if(_nf_flags & nf_open_write)
    {
        if(_nf_flags & nf_open_create && _nf_flags & nf_open_append)
            create_flag = CREATE_NEW;
        else if(_nf_flags & nf_open_create)
            create_flag = CREATE_ALWAYS;
        else if(_nf_flags & nf_open_append)
            create_flag = OPEN_EXISTING;
        else
            create_flag = TRUNCATE_EXISTING;
    }
    int file_attrs = FILE_ATTRIBUTE_NORMAL;
    if(desired_access == GENERIC_READ)
        file_attrs |= FILE_ATTRIBUTE_READONLY;
    return CreateFileW(_filepath.c_str(), desired_access, share_nf_flags, NULL, create_flag, file_attrs, NULL);
#else
    int flags = 0;
    int mode = 0;
    if(_nf_flags & file_open_read && _nf_flags & file_open_write)
        flags = O_RDWR;
    else if(_nf_flags & file_open_write)
        flags = O_WRONLY;
    else
        flags = O_RDONLY;
    if(_nf_flags & file_open_write)
    {
        mode = S_IWUSR | S_IRUSR;
        if(_nf_flags & file_open_create)
            flags |= O_CREAT;
        if(_nf_flags & file_open_append)
            flags |= O_APPEND;
        else
            flags |= O_TRUNC;
    }
    return open(_filepath.c_str(), flags, mode);
#endif
}

void MailUnit::OS::closeNativeFile(MU_NATIVE_FILE _native_file)
{
    if(MU_INVALID_NATIVE_FILE == _native_file)
        return;
#ifdef _WIN32
    CloseHandle(_native_file);
#else
    close(_native_file);
#endif
}

boost::filesystem::path MailUnit::OS::systemConfigDirectory()
{
#ifdef _WIN32
    return fs::path(_wgetenv("ALLUSERSPROFILE")) / BOOST_PP_STRINGIZE(_MU_CONFIG_DIRECTORY);
#elif __APPLE__
#   error Mac OS not supported yet
#else
    return "/etc/" BOOST_PP_STRINGIZE(_MU_CONFIG_DIRECTORY);
#endif
}

boost::filesystem::path MailUnit::OS::userConfigDirectory()
{
#ifdef _WIN32
    return fs::path(_wgetenv("APPDATA")) / BOOST_PP_STRINGIZE(_MU_CONFIG_DIRECTORY);
#elif __APPLE__
#   error Mac OS not supported yet
#else
    return fs::path(getenv("HOME")) / ".config" / BOOST_PP_STRINGIZE(_MU_CONFIG_DIRECTORY);
#endif
}

boost::filesystem::path MailUnit::OS::toAbsolutePath(const boost::filesystem::path & _path,
    const boost::filesystem::path & _app_path)
{
    if(_path.empty())
    {
        return _app_path;
    }
    else if(_path.c_str()[0] == '~')
    {

#ifdef _WIN32
        fs::path home = fs::path(_wgetenv("HOMEDRIVE")) / _wgetenv("HOMEPATH");
#else
        fs::path home = getenv("HOME");
#endif
        return home / &_path.c_str()[1];
    }
    else if(_path.is_relative())
    {
        return fs::absolute(_path, _app_path);
    }
    else
    {
        return _path;
    }
}
