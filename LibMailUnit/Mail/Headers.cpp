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
#include <boost/algorithm/string.hpp>
#include <LibMailUnit/Mail/Headers.h>

using namespace LibMailUnit;
using namespace LibMailUnit::Mail;

HeaderMap::~HeaderMap()
{
    for(Header * header: *this)
        delete header;
}

Header * HeaderMap::find(const std::string & _name) const
{
    for(Header * header: *this)
    {
        if(boost::algorithm::iequals(_name, header->name))
            return header;
    }
    return nullptr;
}


HeaderParser::HeaderParser(std::istream & _input, HeaderMap & _output) :
    mr_input(_input),
    mr_output(_output)
{
}

void HeaderParser::parse(std::istream & _input, HeaderMap & _output)
{
    HeaderParser(_input, _output).parse();
}

void HeaderParser::parse()
{
    std::string line;
    while(std::getline(mr_input, line))
    {
        boost::algorithm::trim_right(line);
        if(!parseLine(line))
            break;
    }
    cleanUp();
}

bool HeaderParser::parseLine(const std::string & _line)
{
    if(_line.empty())
    {
        pushPair();
        return false;
    }
    if(isWhiteSpaceSymbol(_line[0]))
    {
        if(!m_current_key.empty())
            m_current_value += _line;
    }
    else
    {
        pushPair();
        size_t colon_pos = _line.find(':');
        if(std::string::npos != colon_pos)
        {
            m_current_key = _line.substr(0, colon_pos);
            m_current_value = boost::algorithm::trim_copy(_line.substr(colon_pos + 1));
        }
    }
    return true;
}

bool HeaderParser::isWhiteSpaceSymbol(char _symbol) const
{
    switch(_symbol)
    {
    case ' ':
    case '\t':
        return true;
    default:
        return false;
    }
}

void HeaderParser::pushPair()
{
    if(!m_current_key.empty() && !m_current_value.empty())
    {
        Header * header = mr_output.find(m_current_key);
        if(nullptr == header)
        {
            header = new Header(m_current_key);
            header->values.push_back(m_current_value);
            mr_output.push_back(header);
        }
        else
        {
            header->values.push_back(m_current_value);
        }
    }
    cleanUp();
}

void HeaderParser::cleanUp()
{
    m_current_key.clear();
    m_current_value.clear();
}

MU_MailHeaderList * MU_CALL muMailHeadersParseString(const char * _input)
{
    std::stringstream stream(_input);
    HeaderMap * map = new HeaderMap();
    HeaderParser::parse(stream, *map);
    return new MU_MailHeaderList(map, true);
}

MU_MailHeaderList * MU_CALL muMailHeadersParseFile(MU_File _input)
{
    boost::iostreams::file_descriptor fdesc(_input, boost::iostreams::never_close_handle);
    boost::iostreams::stream<boost::iostreams::file_descriptor> stream(fdesc);
    HeaderMap * map = new HeaderMap();
    HeaderParser::parse(stream, *map);
    return new MU_MailHeaderList(map, true);
}

size_t MU_CALL muMailHeadersCount(MU_MailHeaderList * _headers)
{
    const HeaderMap * map = _headers->pointer();
    return nullptr == map ? 0 : map->size();
}

MU_MailHeader * MU_CALL muMailHeaderByIndex(MU_MailHeaderList * _headers, size_t _index)
{
    const HeaderMap * map = _headers->pointer();
    if(nullptr == map || map->size() <= _index)
        return nullptr;
    return new MU_MailHeader((*map)[_index], false);
}

const char * MU_CALL muMailHeaderName(MU_MailHeader * _header)
{
    if(nullptr == _header)
        return nullptr;
    const Header * header = _header->pointer();
    return header->name.c_str();
}

MU_MailHeader * MU_CALL muMailHeaderByName(MU_MailHeaderList * _headers, const char * _name)
{
    if(nullptr == _headers || nullptr == _name)
        return nullptr;
    const HeaderMap * map = _headers->pointer();
    if(nullptr == map)
        return nullptr;
    const Header * header = map->find(_name);
    return nullptr == header ? nullptr : new MU_MailHeader(header, false);
}

size_t MU_CALL muMailHeaderValueCount(MU_MailHeader * _header)
{
    if(nullptr == _header)
        return 0;
    const Header * header = _header->pointer();
    return nullptr == header ? 0 : header->values.size();
}

const char * MU_CALL muMailHeaderValue(MU_MailHeader * _header, size_t _index)
{
    if(nullptr == _header)
        return nullptr;
    const Header * header = _header->pointer();
    return nullptr == header || header->values.size() <= _index ? nullptr :
        header->values[_index].c_str();
}
