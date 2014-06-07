#ifndef __STUBMTP_SMTP_STATEMACHINE_RCPTTOSTATE_H__
#define __STUBMTP_SMTP_STATEMACHINE_RCPTTOSTATE_H__

#include <StubMTP/Smtp/StateMachine/SingleLineCmdState.h>

namespace StubMTP {
namespace Smtp {


class RcptToState : public SingleLineCmdState
{
public:
    RcptToState();

    void processInput(const std::string & _input, Message & _message)  override;

    bool isInutProcessingCompleted() const  override
    {
        return currentState() != ProcessResult::Incomplete;
    }

    bool isProtocolProcessingCompleted() const  override
    {
        return false;
    }

    bool response(ResponseCode * _response) const override;
}; // class RcptToState


} // namespace Smtp
} // namespace StubMTP

#endif // __STUBMTP_SMTP_STATEMACHINE_RCPTTOSTATE_H__
