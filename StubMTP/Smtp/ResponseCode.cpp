#include <StubMTP/Smtp/ResponseCode.h>
#include <StubMTP/Smtp/ProtocolDef.h>

using namespace StubMTP::Smtp;

std::string StubMTP::Smtp::translateResponseCode(ResponseCode _code)
{
    switch(_code)
    {
    case ResponseCode::Ready:
        return "220 [127.0.0.1] Stub Mail Transfer Service Ready" SMTP_NEWLINE;
    case ResponseCode::Closing:
        return "221 [127.0.0.1] Stub Mail Transfer Service Closing" SMTP_NEWLINE;
    case ResponseCode::Ok:
        return "250 OK" SMTP_NEWLINE;
    case ResponseCode::Intermediate:
        return "354 Intermediate" SMTP_NEWLINE;
    case ResponseCode::BadCommandsSequence:
        return "503  Bad sequence of commands" SMTP_NEWLINE;
    case ResponseCode::InvalidParameters:
        return "455 Server unable to accommodate parameters" SMTP_NEWLINE;
    default:
        return "451 Internal server error" SMTP_NEWLINE;
    }
}
