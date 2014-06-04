#ifndef __STUBMTP_SMTP_STATEMACHINE_STATEBASE_H__
#define __STUBMTP_SMTP_STATEMACHINE_STATEBASE_H__

#include <string>
#include <boost/msm/front/states.hpp>
#include "../../Aux.h"
#include "../Message.h"
#include "../ResponseCode.h"

namespace StubMTP {
namespace Smtp {


class StateBase
{
    STUBMTP_DISABLE_COPY(StateBase)

public:
    StateBase() { }
    virtual ~StateBase() { }
    virtual void processInput(const std::string & _input) = 0;
    virtual bool isInutProcessingCompleted() const        = 0;
    virtual bool isProtocolProcessingCompleted() const    = 0;
    virtual void apply(Message & _message) const          = 0;
    virtual bool response(ResponseCode * _response) const = 0;
}; // class StateBase


class State : public boost::msm::front::state<StateBase>
{
}; // class State


} // namespace Smtp
} // namespace StubMTP

#endif // __STUBMTP_SMTP_STATEMACHINE_STATEBASE_H__
