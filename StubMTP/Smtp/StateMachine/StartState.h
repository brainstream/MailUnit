#ifndef __STUBMTP_SMTP_STATEMACHINE_STARTSTATE_H__
#define __STUBMTP_SMTP_STATEMACHINE_STARTSTATE_H__

#include <StubMTP/Smtp/StateMachine/StateBase.h>

namespace StubMTP {
namespace Smtp {


class StartState : public State
{
public:
    void processInput(const std::string & _input, Message & _message)  override
    {
        STUBMTP_UNUSED(_input);
        STUBMTP_UNUSED(_message);
    }

    bool isInutProcessingCompleted() const  override
    {
        return true;
    }

    bool isProtocolProcessingCompleted() const  override
    {
        return false;
    }

    bool response(ResponseCode * _response) const override
    {
        *_response = ResponseCode::Ready;
        return true;
    }

    void reset() override
    {
    }
}; // class StartState


} // namespace Smtp
} // namespace StubMTP

#endif // __STUBMTP_SMTP_STATEMACHINE_STARTSTATE_H__
