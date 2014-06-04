#ifndef __STUBMTP_SMTP_STATEMACHINE_QUITSTATE_H__
#define __STUBMTP_SMTP_STATEMACHINE_QUITSTATE_H__

#include "StateBase.h"

namespace StubMTP {
namespace Smtp {


class QuitState : public State
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
        return true;
    }

    void apply(Message & _message) const  override
    {
        STUBMTP_UNUSED(_message);
    }

    bool response(ResponseCode * _response) const override
    {
        *_response = ResponseCode::Closing;
        return true;
    }
}; // class QuitState


} // namespace Smtp
} // namespace StubMTP

#endif // __STUBMTP_SMTP_STATEMACHINE_QUITSTATE_H__
