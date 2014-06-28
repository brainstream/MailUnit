/***********************************************************************************************
 *                                                                                             *
 * This file is part of MailUnit.                                                               *
 *                                                                                             *
 * MailUnit is free software: you can redistribute it and/or modify it under the terms of       *
 * the GNU General Public License as published by the Free Software Foundation,                *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * MailUnit is distributed in the hope that it will be useful,  but WITHOUT ANY WARRANTY;       *
 * without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  *
 * See the GNU General Public License for more details.                                        *
 *                                                                                             *
 * You should have received a copy of the GNU General Public License along with MailUnit.       *
 * If not, see <http://www.gnu.org/licenses/>.                                                 *
 *                                                                                             *
 ***********************************************************************************************/

#include <MailUnit/Server/Email/Header.h>

using namespace MailUnit::Email;

namespace {

class HeaderParser
{
private:
    inline HeaderParser(std::istream & _input, HeaderMap & _output);

public:
    static inline void parse(std::istream & _input, HeaderMap & _output);

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

void MailUnit::Email::parseHeaders(std::istream & _input, HeaderMap & _output)
{
    HeaderParser::parse(_input, _output);
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
    return ' ' == _symbol || '\t' == _symbol;
}

void HeaderParser::pushPair()
{
    if(!m_current_key.empty() && !m_current_value.empty())
    {
        HeaderMap::iterator it = mr_output.find(m_current_key);
        if(mr_output.end() == it)
        {
            mr_output[m_current_key] = std::vector<std::string>({ m_current_value });
        }
        else
        {
            it->second.push_back(m_current_value);
        }
    }
    cleanUp();
}

void HeaderParser::cleanUp()
{
    m_current_key.clear();
    m_current_value.clear();
}
