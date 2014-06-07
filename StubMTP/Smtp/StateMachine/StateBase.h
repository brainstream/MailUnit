#ifndef __STUBMTP_SMTP_STATEMACHINE_STATEBASE_H__
#define __STUBMTP_SMTP_STATEMACHINE_STATEBASE_H__

#include <string>
#include <boost/msm/front/states.hpp>
#include <StubMTP/Aux.h>
#include <StubMTP/Smtp/Message.h>
#include <StubMTP/Smtp/ResponseCode.h>

namespace StubMTP {
namespace Smtp {


class StateBase
{
    STUBMTP_DISABLE_COPY(StateBase)

public:
    StateBase()
    {
    }

    virtual ~StateBase()
    {
    }

    virtual void processInput(const std::string & _input, Message & _message) = 0;
    virtual bool isInutProcessingCompleted() const                            = 0;
    virtual bool isProtocolProcessingCompleted() const                        = 0;
    virtual bool response(ResponseCode * _response) const                     = 0;
}; // class StateBase


class State : public boost::msm::front::state<StateBase>
{
public:
    template <typename EventT, typename FsmT>
    void on_entry(const EventT & _event, FsmT &)
    {
        processInput(_event.data(), _event.message());
    }

    template <typename EventT, typename FsmT>
    void on_exit(const EventT & _event, FsmT &)
    {
        reset();
    }

protected:
    virtual void reset() = 0;
}; // class State


} // namespace Smtp
} // namespace StubMTP

#endif // __STUBMTP_SMTP_STATEMACHINE_STATEBASE_H__
