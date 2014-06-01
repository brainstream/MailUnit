#ifndef __STUBMTP_SMTP_SESSIONPROVIDER_H__
#define __STUBMTP_SMTP_SESSIONPROVIDER_H__

#include <memory>
#include <boost/asio.hpp>
#include "Message.h"

namespace StubMTP {
namespace Smtp {

class SessionProvider : public std::enable_shared_from_this<SessionProvider>
{
public:
    explicit SessionProvider(boost::asio::io_service & _io_service); // TODO: delete service from args
    virtual ~SessionProvider();
    void startNewSession(boost::asio::ip::tcp::socket _socket);
    virtual void onMessageRecieved(const Message & _message) = 0;
    virtual void onFail() = 0;

private:
    boost::asio::io_service & mr_io_service;
}; // class SessionProvider

} // namespace Smtp
} // namespace StubMTP

#endif // __STUBMTP_SMTP_SESSIONPROVIDER_H__
