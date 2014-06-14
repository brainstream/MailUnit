#ifndef __STUBMPT_EMAIL_HEADER_H__
#define __STUBMPT_EMAIL_HEADER_H__

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
