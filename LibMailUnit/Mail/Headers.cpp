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
#include <boost/algorithm/string.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>
#include <Include/LibMailUnit/Mail.h>
#include <LibMailUnit/Memory.h>
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

std::shared_ptr<HeaderMap> HeaderParser::parse(std::istream & _input)
{
    HeaderMap * map = new HeaderMap();
    HeaderParser(_input, *map).parse();
    return std::shared_ptr<HeaderMap>(map);
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

MU_MAIL_HEADERLIST MU_CALL muMailHeadersParseString(const char * _input)
{
    std::stringstream stream(_input);
    std::shared_ptr<HeaderMap> map_sptr = HeaderParser::parse(stream);
    HeaderMap * map = map_sptr.get();
    map_sptr.reset();
    return new MHandle(map, true);
}

MU_MAIL_HEADERLIST MU_CALL muMailHeadersParseFile(MU_NATIVE_FILE _input)
{
    boost::iostreams::file_descriptor fdesc(_input, boost::iostreams::never_close_handle);
    boost::iostreams::stream<boost::iostreams::file_descriptor> stream(fdesc);
    std::shared_ptr<HeaderMap> map_sptr = HeaderParser::parse(stream);
    HeaderMap * map = map_sptr.get();
    map_sptr.reset();
    return new MHandle(map, true);
}

size_t MU_CALL muMailHeadersCount(MU_MAIL_HEADERLIST _headers)
{
    HeaderMap * map = _headers->pointer<HeaderMap>();
    return nullptr == map ? 0 : map->size();
}

MU_MAIL_HEADER MU_CALL muMailHeaderByIndex(MU_MAIL_HEADERLIST _headers, size_t _index)
{
    HeaderMap * map = _headers->pointer<HeaderMap>();
    if(nullptr == map || map->size() <= _index)
        return MU_INVALID_HANDLE;
    return new MHandle((*map)[_index], false);
}

MU_MAIL_HEADER MU_CALL muMailHeaderByName(MU_MAIL_HEADERLIST _headers, const char * _name)
{
    HeaderMap * map = _headers->pointer<HeaderMap>();
    if(nullptr == map)
        return MU_INVALID_HANDLE;
    Header * header = map->find(_name);
    return nullptr == header ? MU_INVALID_HANDLE : new MHandle(header, false);
}

size_t MU_CALL muMailHeaderValueCount(MU_MAIL_HEADER _header)
{
    Header * header = _header->pointer<Header>();
    return nullptr == header ? 0 : header->values.size();
}

const char * MU_CALL muMailHeaderValue(MU_MAIL_HEADER _header, size_t _index)
{
    Header * header = _header->pointer<Header>();
    return nullptr == header || header->values.size() <= _index ? nullptr :
        header->values[_index].c_str();
}
