/***********************************************************************************************
 *                                                                                             *
 * This file is part of the MailUnit Library.                                                  *
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

#ifndef __LIBMU_MESSAGE_HEADERS_H__
#define __LIBMU_MESSAGE_HEADERS_H__

#include <string>
#include <vector>
#include <istream>
#include <memory>

namespace LibMailUnit {
namespace Message {

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
    ~HeaderMap();
    Header * find(const std::string & _name) const;
}; // class HeaderMap

class HeaderParser
{
private:
    HeaderParser(std::istream & _input, HeaderMap & _output);

public:
    static void parse(std::istream & _input, HeaderMap &_output);

private:
    void parse();
    bool parseLine(const std::string & _line);
    bool isWhiteSpaceSymbol(char _symbol) const;
    void pushPair();
    void cleanUp();

private:
    std::istream & mr_input;
    HeaderMap & mr_output;
    std::string m_current_key;
    std::string m_current_value;
}; // class HeaderParser

} // namespace Message
} // namespace LibMailUnit

#endif // __LIBMU_MESSAGE_HEADERS_H__
