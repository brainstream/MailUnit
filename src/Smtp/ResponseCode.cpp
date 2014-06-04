#include "ResponseCode.h"

using namespace StubMTP::Smtp;

std::string StubMTP::Smtp::translateResponseCode(ResponseCode _code)
{
    switch(_code)
    {
    case ResponseCode::Ready:
        return "220 [127.0.0.1] Stub Mail Transfer Service Ready\r\n";
    case ResponseCode::Closing:
        return "221 [127.0.0.1] Stub Mail Transfer Service Closing\r\n";
    case ResponseCode::Ok:
        return "250 OK\r\n";
    case ResponseCode::Intermediate:
        return "354 Intermediate\r\n";
    case ResponseCode::BadCommandsSequence:
        return "503  Bad sequence of commands\r\n";
    default:
        return "451 Internal server error\r\n";
    }
}
