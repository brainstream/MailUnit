#ifndef __STUBMTP_SMTPCONTROLLER_H__
#define __STUBMTP_SMTPCONTROLLER_H__

#include "Smtp/SessionProvider.h"

namespace StubMTP {

class SmtpController : public Smtp::SessionProvider
{
public:
    explicit SmtpController(boost::asio::io_service & _io_service);
    void onMessageRecieved(const Smtp::Message & _message) override;
    void onFail() override;
}; // class SmtpController

} // namespace StubMTP

#endif // __STUBMTP_SMTPCONTROLLER_H__
