#ifndef __STUBMTP_SMTP_STATEMACHINE_EHLOSTATE_H__
#define __STUBMTP_SMTP_STATEMACHINE_EHLOSTATE_H__

#include "StateBase.h"

namespace StubMTP {
namespace Smtp {


class EhloState : public State
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
        *_response = ResponseCode::Ok;
        return true;
    }
}; // class EhloState


} // namespace Smtp
} // namespace StubMTP

#endif // __STUBMTP_SMTP_STATEMACHINE_EHLOSTATE_H__
