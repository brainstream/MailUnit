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

#include <MailUnit/OS.h>
#ifdef _WIN32
#   include <windows.h>
#else
#   include <fcntl.h>
#endif


MU_NATIVE_FILE MailUnit::openNativeFile(const boost::filesystem::path & _filepath, int _flags)
{
#ifdef _WIN32
    int desired_access = 0;
    int share_flags = FILE_SHARE_READ;
    int create_flag = OPEN_EXISTING;
    if(_flags & open_nf_read && _flags & open_nf_write)
        desired_access = GENERIC_ALL;
    else if(_flags & open_nf_read)
        desired_access = GENERIC_READ;
    else if(_flags & open_nf_write)
        desired_access = GENERIC_WRITE;
    else
        return INVALID_HANDLE_VALUE;
    if(_flags & open_nf_write)
    {
        if(_flags & open_nf_create && _flags & open_nf_append)
            create_flag = CREATE_NEW;
        else if(_flags & open_nf_create)
            create_flag = CREATE_ALWAYS;
        else if(_flags & open_nf_append)
            create_flag = OPEN_EXISTING;
        else
            create_flag = TRUNCATE_EXISTING;
    }
    int file_attrs = FILE_ATTRIBUTE_NORMAL;
    if(desired_access == GENERIC_READ)
        file_attrs |= FILE_ATTRIBUTE_READONLY;
    return CreateFileW(_filepath.c_str(), desired_access, share_flags, NULL, create_flag, file_attrs, NULL);
#else
    int open_flags = 0;
    if(_flags & open_nf_read && _flags & open_nf_write)
        open_flags = O_RDWR;
    else if(_flags & open_nf_read)
        open_flags = O_RDONLY;
    else if(_flags & open_nf_write)
        open_flags = O_WRONLY;
    else
        return -1;
    if(_flags & open_nf_write)
    {
        if(_flags & open_nf_create)
            open_flags |= O_CREAT;
        if(_flags & open_nf_append)
            open_flags |= O_APPEND;
        else
            open_flags |= O_TRUNC;
    }
    return open(_filepath.c_str(), open_flags);
#endif
}

void MailUnit::closeNativeFile(MU_NATIVE_FILE _file)
{
#ifdef _WIN32
    CloseHandle(_file);
#else
    close(_file);
#endif
}
