#ifndef __STUBMTP_SMTP_MESSAGE_H__
#define __STUBMTP_SMTP_MESSAGE_H__

#include <string>
#include <vector>

namespace StubMTP {
namespace Smtp {

struct Message
{
    std::string from;
    std::vector<std::string> to;
    std::vector<std::string> cc;
    std::string body;
}; // struct Message

} // namespace Smtp
} // namespace StubMTP

#endif // __STUBMTP_SMTP_MESSAGE_H__
