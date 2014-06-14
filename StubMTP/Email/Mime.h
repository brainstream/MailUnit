#ifndef __STUBMTP_EMAIL_MIME_H__
#define __STUBMTP_EMAIL_MIME_H__

#include <memory>
#include <string>
#include <vector>
#include <StubMTP/Smtp/Message.h>
#include <StubMTP/Email/Header.h>
#include <StubMTP/Email/Address.h>
#include <StubMTP/Email/DateTime.h>


namespace StubMTP {
namespace Email {

struct MailContent
{
    std::string type;
    std::string encoding;
    HeaderMap all_headers;
    std::string content;
}; // struct MailContent


// RFC 5322 3.6 - http://tools.ietf.org/html/rfc5322#section-3.6
// RFC 6854 - http://tools.ietf.org/html/rfc6854
struct Mime
{
    // TODO: return         http://tools.ietf.org/html/rfc5322#section-3.6.7
    // TODO: received       http://tools.ietf.org/html/rfc5322#section-3.6.7
    // TODO: resent-date    http://tools.ietf.org/html/rfc5322#section-3.6.6
    // TODO: resent-from    http://tools.ietf.org/html/rfc5322#section-3.6.6
    // TODO: resent-sender  http://tools.ietf.org/html/rfc5322#section-3.6.6
    // TODO: resent-to      http://tools.ietf.org/html/rfc5322#section-3.6.6
    // TODO: resent-cc      http://tools.ietf.org/html/rfc5322#section-3.6.6
    // TODO: resent-bcc     http://tools.ietf.org/html/rfc5322#section-3.6.6
    // TODO: resent-msg-id  http://tools.ietf.org/html/rfc5322#section-3.6.6
    // TODO: in-reply-to    http://tools.ietf.org/html/rfc5322#section-3.6.4
    // TODO: references     http://tools.ietf.org/html/rfc5322#section-3.6.4
    // TODO: comments       http://tools.ietf.org/html/rfc5322#section-3.6.5
    // TODO: keywords       http://tools.ietf.org/html/rfc5322#section-3.6.5
    std::string message_id; // TODO: parse? http://tools.ietf.org/html/rfc5322#section-3.6.4
    DateTimePtr date;
    std::string user_agent;
    std::string mime_version;
    std::string subject;
    AddressGroupPtr sender;
    AddressGroupPtr from;
    AddressGroupPtr to;
    AddressGroupPtr cc;
    AddressGroupPtr bcc;
    AddressGroupPtr reply_to;
    HeaderMap all_headers;
    std::vector<MailContent> contents;
}; // struct Mime

std::shared_ptr<Mime> parseMime(const Smtp::Message & _message);

} // namespace Email
} // namespace StubMTP


#endif // __STUBMTP_EMAIL_MIME_H__
