#ifndef __STUBMPT_EMAIL_HEADER_H__
#define __STUBMPT_EMAIL_HEADER_H__

#include <map>
#include <string>
#include <istream>
#include <boost/algorithm/string.hpp>

namespace StubMTP {
namespace Email {


struct HeaderKey
{
    static const char * const message_id;
    static const char * const sender;
    static const char * const from;
    static const char * const to;
    static const char * const cc;
    static const char * const bcc;
    static const char * const reply_to;
    static const char * const date;
    static const char * const subject;
    static const char * const user_agent;
    static const char * const mime_version;
}; // struct HeaderKey

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
