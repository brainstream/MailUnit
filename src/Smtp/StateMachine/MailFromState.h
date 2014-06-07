#ifndef __STUBMTP_SMTP_STATEMACHINE_MAILFROMSTATE_H__
#define __STUBMTP_SMTP_STATEMACHINE_MAILFROMSTATE_H__

#include "SingleLineCmdState.h"

namespace StubMTP {
namespace Smtp {


class MailFromState : public SingleLineCmdState
{
public:
    MailFromState();

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
}; // class MailFromState


} // namespace Smtp
} // namespace StubMTP

#endif // __STUBMTP_SMTP_STATEMACHINE_MAILFROMSTATE_H__
