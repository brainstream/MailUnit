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

#include <string>
#include <sstream>
#include <map>
#include <unordered_map>
#include <vector>
#include <istream>
#include <boost/algorithm/string.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>
#include <LibMailUnit/InternalMemory.h>
#include <LibMailUnit/Mail/Headers.h>

using namespace LibMailUnit;

namespace {

struct Header
{
    Header(const std::string & _name) :
        name(_name)
    {
    }

    const std::string name;
    std::vector<std::string> values;
}; // struct Header

class HeaderMap : public std::vector<Header *>
{
public:
    inline ~HeaderMap();
    Header * find(const std::string & _name) const;
}; // class HeaderMap

class HeaderParser
{
private:
    inline HeaderParser(std::istream & _input, HeaderMap & _output);

public:
    static MU_MAIL_HEADERLIST parse(std::istream & _input);

private:
    void parse();
    bool parseLine(const std::string & _line);
    inline bool isWhiteSpaceSymbol(char _symbol) const;
    void pushPair();
    void cleanUp();

private:
    std::istream & mr_input;
    HeaderMap & mr_output;
    std::string m_current_key;
    std::string m_current_value;
}; // class HeaderParser

} // namespace


HeaderMap::~HeaderMap()
{
    for(Header * header: *this)
        delete header;
}

Header * HeaderMap::find(const std::string & _name) const
{
    for(Header * header: *this)
    {
        if(boost::algorithm::ilexicographical_compare(_name, header->name))
            return header;
    }
    return nullptr;
}


HeaderParser::HeaderParser(std::istream & _input, HeaderMap & _output) :
    mr_input(_input),
    mr_output(_output)
{
}

MU_MAIL_HEADERLIST  HeaderParser::parse(std::istream & _input)
{
    MU_HANDLE handle = makeHandle<HeaderMap>();
    HeaderMap * map = handlePointer<HeaderMap>(handle);
    HeaderParser(_input, *map).parse();
    return handle;
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

MU_MAIL_HEADERLIST muMailHeadersParseString(const char * _input)
{
    std::stringstream stream(_input);
    return HeaderParser::parse(stream);
}

MU_MAIL_HEADERLIST muMailHeadersParseFile(MU_NATIVE_FILE _input)
{
    boost::iostreams::file_descriptor fdesc(_input, boost::iostreams::never_close_handle);
    boost::iostreams::stream<boost::iostreams::file_descriptor> stream(fdesc);
    return HeaderParser::parse(stream);
}

size_t muMailHeadersCount(MU_MAIL_HEADERLIST _headers)
{
    HeaderMap * map = handlePointer<HeaderMap>(_headers);
    return nullptr == map ? 0 : map->size();
}

MU_MAIL_HEADER muMailHeaderByIndex(MU_MAIL_HEADERLIST _headers, size_t _index)
{
    HeaderMap * map = handlePointer<HeaderMap>(_headers);
    if(nullptr == map || map->size() <= _index)
        return MU_INVALID_HANDLE;
    return muWrapPointer((*map)[_index]);
}

MU_MAIL_HEADER muMailHeaderByName(MU_MAIL_HEADERLIST _headers, const char * _name)
{
    HeaderMap * map = handlePointer<HeaderMap>(_headers);
    if(nullptr == map)
        return MU_INVALID_HANDLE;
    Header * header = map->find(_name);
    return nullptr == header ? MU_INVALID_HANDLE : muWrapPointer(header);
}

size_t muMailHeaderValueCount(MU_MAIL_HEADER _header)
{
    Header * header = handlePointer<Header>(_header);
    return nullptr == header ? 0 : header->values.size();
}

const char * muMailHeaderValue(MU_MAIL_HEADER _header, size_t _index)
{
    Header * header = handlePointer<Header>(_header);
    return nullptr == header || header->values.size() <= _index ? nullptr :
        header->values[_index].c_str();
}
