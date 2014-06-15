/***********************************************************************************************
 *                                                                                             *
 * This file is part of StubMTP.                                                               *
 *                                                                                             *
 * StubMTP is free software: you can redistribute it and/or modify it under the terms of       *
 * the GNU General Public License as published by the Free Software Foundation,                *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * StubMTP is distributed in the hope that it will be useful,  but WITHOUT ANY WARRANTY;       *
 * without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  *
 * See the GNU General Public License for more details.                                        *
 *                                                                                             *
 * You should have received a copy of the GNU General Public License along with StubMTP.       *
 * If not, see <http://www.gnu.org/licenses/>.                                                 *
 *                                                                                             *
 ***********************************************************************************************/

#ifndef __STUBMPT_EMAIL_HEADER_H__
#define __STUBMPT_EMAIL_HEADER_H__

/*----------------------------------------+----------------------------------------------------+
 | RFC 5322 - Internet Message Format     | http://tools.ietf.org/html/rfc5322#section-2.2     |
 |     section 2.2 - Header Fields        |                                                    |
 +----------------------------------------+----------------------------------------------------*/

#include <map>
#include <vector>
#include <string>
#include <istream>
#include <boost/algorithm/string.hpp>

namespace StubMTP {
namespace Email {

struct HeaderKeyComparer
{
    bool operator ()(const std::string & _left, const std::string & _right) const
    {
        return boost::algorithm::ilexicographical_compare(_left, _right);
    }
}; // struct HeaderKeyComparer

typedef std::map<std::string, std::vector<std::string>, struct HeaderKeyComparer> HeaderMap;

void parseHeaders(std::istream & _input, HeaderMap & _output);

} // namespace Email
} // namespace StubMTP

#endif // __STUBMPT_EMAIL_HEADER_H__
