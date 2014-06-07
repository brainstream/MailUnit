#include <StubMTP/Smtp/StateMachine/MailFromState.h>
#include <StubMTP/Smtp/ProtocolDef.h>

#define COMMAND SMTP_CMD_MAIL " FROM:"

using namespace StubMTP::Smtp;

MailFromState::MailFromState() :
    SingleLineCmdState(sizeof(COMMAND) - 1)
{
}

void MailFromState::processInput(const std::string & _input, Message & _message)
{
    if(internalProcessInput(_input) == ProcessResult::Success)
        commandArgString(_message.from);
}

bool MailFromState::response(ResponseCode * _response) const
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
