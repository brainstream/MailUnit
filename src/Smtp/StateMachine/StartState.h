#ifndef __STUBMTP_SMTP_STATEMACHINE_STARTSTATE_H__
#define __STUBMTP_SMTP_STATEMACHINE_STARTSTATE_H__

#include "StateBase.h"

namespace StubMTP {
namespace Smtp {


class StartState : public State
{
public:
    void processInput(const std::string & _input)  override
    {
        STUBMTP_UNUSED(_input);
    }

    bool isInutProcessingCompleted() const  override
    {
        return true;
    }

    bool isProtocolProcessingCompleted() const  override
    {
        return false;
    }

    void apply(Message & _message) const  override
    {
        STUBMTP_UNUSED(_message);
    }

    bool response(ResponseCode * _response) const override
    {
        *_response = ResponseCode::Ready;
        return true;
    }
}; // class StartState


} // namespace Smtp
} // namespace StubMTP

#endif // __STUBMTP_SMTP_STATEMACHINE_STARTSTATE_H__
