#ifndef __STUBMTP_SMTP_STATEMACHINE_QUITSTATE_H__
#define __STUBMTP_SMTP_STATEMACHINE_QUITSTATE_H__

#include "StateBase.h"

namespace StubMTP {
namespace Smtp {


class QuitState : public State
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
        return true;
    }

    bool response(ResponseCode * _response) const override
    {
        *_response = ResponseCode::Closing;
        return true;
    }

    void reset() override
    {
    }
}; // class QuitState


} // namespace Smtp
} // namespace StubMTP

#endif // __STUBMTP_SMTP_STATEMACHINE_QUITSTATE_H__
