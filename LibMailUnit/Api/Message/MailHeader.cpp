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
#include <LibMailUnit/Api/Message/MailHeader.h>

using namespace LibMailUnit::Mail;

const MU_MailHeaderList * muMailHeadersParseString(const char * _input)
{
    std::stringstream stream(_input);
    HeaderMap * map = new HeaderMap();
    HeaderParser::parse(stream, *map);
    return new MU_MailHeaderList(map, true);
}

const MU_MailHeaderList * muMailHeadersParseFile(MU_NATIVE_FILE _input)
{
    boost::iostreams::file_descriptor fdesc(_input, boost::iostreams::never_close_handle);
    boost::iostreams::stream<boost::iostreams::file_descriptor> stream(fdesc);
    HeaderMap * map = new HeaderMap();
    HeaderParser::parse(stream, *map);
    return new MU_MailHeaderList(map, true);
}

size_t MU_CALL muMailHeadersCount(const MU_MailHeaderList * _headers)
{
    const HeaderMap * map = _headers->pointer();
    return nullptr == map ? 0 : map->size();
}

const MU_MailHeader * muMailHeaderByIndex(const MU_MailHeaderList * _headers, size_t _index)
{
    const HeaderMap * map = _headers->pointer();
    if(nullptr == map || map->size() <= _index)
        return nullptr;
    return new MU_MailHeader((*map)[_index], false);
}

const char * MU_CALL muMailHeaderName(const MU_MailHeader * _header)
{
    if(nullptr == _header)
        return nullptr;
    const Header * header = _header->pointer();
    return header->name.c_str();
}

const MU_MailHeader * muMailHeaderByName(const MU_MailHeaderList * _headers, const char * _name)
{
    if(nullptr == _headers || nullptr == _name)
        return nullptr;
    const HeaderMap * map = _headers->pointer();
    if(nullptr == map)
        return nullptr;
    const Header * header = map->find(_name);
    return nullptr == header ? nullptr : new MU_MailHeader(header, false);
}

size_t MU_CALL muMailHeaderValueCount(const MU_MailHeader * _header)
{
    if(nullptr == _header)
        return 0;
    const Header * header = _header->pointer();
    return nullptr == header ? 0 : header->values.size();
}

const char * MU_CALL muMailHeaderValue(const MU_MailHeader * _header, size_t _index)
{
    if(nullptr == _header)
        return nullptr;
    const Header * header = _header->pointer();
    return nullptr == header || header->values.size() <= _index ? nullptr :
        header->values[_index].c_str();
}
