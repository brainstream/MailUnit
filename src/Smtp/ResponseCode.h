#ifndef __STUBMTP_SMTP_RESPONSECODE_H__
#define __STUBMTP_SMTP_RESPONSECODE_H__

#include <string>

namespace StubMTP {
namespace Smtp {


enum class ResponseCode
{
    Status                         = 211,
    Help                           = 214,
    Ready                          = 220,
    Closing                        = 221,
    Ok                             = 250,
    Forward                        = 251,
    UserNotVerified                = 252,
    Intermediate                   = 354,
    ServiceNotAvailable            = 421,
    MailboxAnavailable             = 450,
    InternalError                  = 451,
    InsufficientSystemStorage      = 452,
    InvalidParameters              = 455,
    UnrecognizedCommand            = 500,
    UnrecognizedParameters         = 501,
    CommandNotImplemented          = 502,
    BadCommandsSequence            = 503,
    CommandParameterNotImplemented = 504,
    MailboxUnavailable             = 550,
    ForvardPath                    = 551,
    MailActionAborted              = 552,
    MailboxNameNotAllowed          = 553,
    TransactionFailed              = 554,
    MailAddressNotRecognized       = 555
}; // enum class ResponseCode


std::string translateResponseCode(ResponseCode _code);


} // namespace Smtp
} // namespace StubMTP

#endif // __STUBMTP_SMTP_RESPONSECODE_H__
