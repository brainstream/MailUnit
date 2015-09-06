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

#include <sstream>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>
#include <LibMailUnit/Memory.h>
#include <LibMailUnit/Mail/Headers.h>
#include <Include/LibMailUnit/Message/MailHeader.h>

using namespace LibMailUnit::Mail;

MU_MAIL_HEADERLIST MU_CALL muMailHeadersParseString(const char * _input)
{
    std::stringstream stream(_input);
    HeaderMap * map = new HeaderMap();
    HeaderParser::parse(stream, *map);
    return new MHandle(const_cast<const HeaderMap *>(map), true);
}

MU_MAIL_HEADERLIST MU_CALL muMailHeadersParseFile(MU_NATIVE_FILE _input)
{
    boost::iostreams::file_descriptor fdesc(_input, boost::iostreams::never_close_handle);
    boost::iostreams::stream<boost::iostreams::file_descriptor> stream(fdesc);
    HeaderMap * map = new HeaderMap();
    HeaderParser::parse(stream, *map);
    return new MHandle(const_cast<const HeaderMap *>(map), true);
}

size_t MU_CALL muMailHeadersCount(MU_MAIL_HEADERLIST _headers)
{
    const HeaderMap * map = _headers->pointer<const HeaderMap>();
    return nullptr == map ? 0 : map->size();
}

MU_MAIL_HEADER MU_CALL muMailHeaderByIndex(MU_MAIL_HEADERLIST _headers, size_t _index)
{
    const HeaderMap * map = _headers->pointer<const HeaderMap>();
    if(nullptr == map || map->size() <= _index)
        return MU_INVALID_HANDLE;
    return new MHandle(const_cast<const Header *>((*map)[_index]), false);
}

const char * MU_CALL muMailHeaderName(MU_MAIL_HEADER _header)
{
    if(nullptr == _header || MU_INVALID_HANDLE == _header)
        return nullptr;
    const Header * header = _header->pointer<const Header>();
    return header->name.c_str();
}

MU_MAIL_HEADER MU_CALL muMailHeaderByName(MU_MAIL_HEADERLIST _headers, const char * _name)
{
    const HeaderMap * map = _headers->pointer<const HeaderMap>();
    if(nullptr == map)
        return MU_INVALID_HANDLE;
    const Header * header = map->find(_name);
    return nullptr == header ? MU_INVALID_HANDLE : new MHandle(header, false);
}

size_t MU_CALL muMailHeaderValueCount(MU_MAIL_HEADER _header)
{
    const Header * header = _header->pointer<const Header>();
    return nullptr == header ? 0 : header->values.size();
}

const char * MU_CALL muMailHeaderValue(MU_MAIL_HEADER _header, size_t _index)
{
    const Header * header = _header->pointer<const Header>();
    return nullptr == header || header->values.size() <= _index ? nullptr :
        header->values[_index].c_str();
}
