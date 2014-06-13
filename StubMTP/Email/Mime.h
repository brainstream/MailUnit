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

struct Mime
{
    std::string message_id;
    DateTimePtr date;
    std::string user_agent;
    std::string mime_version;
    std::string subject;
    Address sender;
    AddressGroup from;
    AddressGroup to;
    AddressGroup cc;
    AddressGroup bcc;
    AddressGroup reply_to;
    HeaderMap all_headers;
    std::vector<MailContent> contents;
}; // struct Mime

std::shared_ptr<Mime> parseMime(const Smtp::Message & _message);

} // namespace Email
} // namespace StubMTP


#endif // __STUBMTP_EMAIL_MIME_H__
