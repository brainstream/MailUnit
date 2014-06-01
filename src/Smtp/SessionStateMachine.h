#ifndef __STUBMTP_SMTP_SESSIONSTATEMACHINE_H__
#define __STUBMTP_SMTP_SESSIONSTATEMACHINE_H__

#include <string>
#include "../Aux.h"

namespace StubMTP {
namespace Smtp {

class SessionStateMachine final
{
    STUBMTP_DISABLE_COPY(SessionStateMachine)
    struct Private;

public:
    struct Response
    {
        bool ready;
        std::string message;
    };

public:
    SessionStateMachine();
    ~SessionStateMachine();
    Response pushRequest(const std::string & _string);

private:
    Private * mp_private;
}; // class SessionStateMachine

} // namespace Smtp
} // namespace StubMTP

#endif // __STUBMTP_SMTP_SESSIONSTATEMACHINE_H__
