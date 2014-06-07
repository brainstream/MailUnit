#include "RcptToState.h"
#include "../ProtocolDef.h"

#define COMMAND SMTP_CMD_RCPT " TO:"

using namespace StubMTP::Smtp;

RcptToState::RcptToState() :
    SingleLineCmdState(sizeof(COMMAND) - 1)
{
}

void RcptToState::processInput(const std::string & _input, Message & _message)
{
    if(internalProcessInput(_input) == ProcessResult::Success)
    {
        std::string to;
        if(commandArgString(to))
            _message.to.push_back(to);
    }
}

bool RcptToState::response(ResponseCode * _response) const
{
    switch(currentState())
    {
    case ProcessResult::Incomplete:
        return false;
    case ProcessResult::Error:
        *_response = ResponseCode::InvalidParameters;
        return true;
    default:
        *_response = ResponseCode::Ok;
        return true;
    }
}
