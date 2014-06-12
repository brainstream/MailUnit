#ifndef __STUBMTP_EMAIL_MIME_H__
#define __STUBMTP_EMAIL_MIME_H__

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <boost/date_time.hpp>
#include <boost/algorithm/string.hpp>
#include <StubMTP/Smtp/Message.h>
#include <StubMTP/Email/Address.h>


namespace StubMTP {
namespace Email {

namespace MimeHeaderKey {

const char * const from         = "From";
const char * const to           = "To";
const char * const cc           = "Cc";
const char * const bcc          = "Bcc";
const char * const message_id   = "Message-ID";
const char * const date         = "Date";
const char * const user_agent   = "User-Agent";
const char * const mime_version = "MIME-Version";
const char * const subject      = "Subject";

} // namespace MimeHeaderKey

struct MimeHeaderKeyComparer
{
    bool operator ()(const std::string & _left, const std::string & _right) const
    {
        return boost::algorithm::ilexicographical_compare(_left, _right);
    }
}; // struct MimeHeaderKeyComparer

typedef std::map<std::string, std::string, struct MimeHeaderKeyComparer> MimeHeaderMap;

struct MailContent
{
    std::string type;
    std::string encoding;
    MimeHeaderMap all_headers;
    std::string content;
}; // struct MailContent

struct Mime
{
    std::string message_id;
    boost::posix_time::ptime date;
    std::string user_agent;
    std::string mime_version;
    std::string subject;
    Address from;
    AddressGroup to;
    AddressGroup cc;
    AddressGroup bcc;
    AddressGroup reply_to;
    MimeHeaderMap all_headers;
    std::vector<MailContent> contents;
}; // struct Mime

std::shared_ptr<Mime> parseMime(const Smtp::Message & _message);

} // namespace Email
} // namespace StubMTP

#endif // __STUBMTP_EMAIL_MIME_H__
