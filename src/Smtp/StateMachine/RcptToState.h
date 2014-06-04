#ifndef __STUBMTP_SMTP_STATEMACHINE_RCPTTOSTATE_H__
#define __STUBMTP_SMTP_STATEMACHINE_RCPTTOSTATE_H__

#include "StateBase.h"

namespace StubMTP {
namespace Smtp {


class RcptToState : public State
{
public:
    void processInput(const std::string & _input)  override
    {
        // TODO: implement!
        STUBMTP_UNUSED(_input);
    }

    bool isInutProcessingCompleted() const  override
    {
        // TODO: implement!
        return true;
    }

    bool isProtocolProcessingCompleted() const  override
    {
        return false;
    }

    void apply(Message & _message) const  override
    {
        // TODO: implement!
        STUBMTP_UNUSED(_message);
    }

    bool response(ResponseCode * _response) const override
    {
        // TODO: implement!
        *_response = ResponseCode::Ok;
        return true;
    }
}; // class RcptToState


} // namespace Smtp
} // namespace StubMTP

#endif // __STUBMTP_SMTP_STATEMACHINE_RCPTTOSTATE_H__
