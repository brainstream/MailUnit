#ifndef __STUBMPT_EMAIL_HEADER_H__
#define __STUBMPT_EMAIL_HEADER_H__

/*
+------------------------------------+-------------------------------------------------+
| RFC 5322 - Internet Message Format | http://tools.ietf.org/html/rfc5322#section-2.2  |
|     section 2.2 - Header Fields    |                                                 |
+------------------------------------+-------------------------------------------------+
*/

#include <map>
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

typedef std::map<std::string, std::string, struct HeaderKeyComparer> HeaderMap;

void parseHeaders(std::istream & _input, HeaderMap & _output);

} // namespace Email
} // namespace StubMTP

#endif // __STUBMPT_EMAIL_HEADER_H__
