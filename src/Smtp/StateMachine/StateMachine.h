#ifndef __STUBMTP_SMTP_STATEMACHINE_STATEMACHINE_H__
#define __STUBMTP_SMTP_STATEMACHINE_STATEMACHINE_H__

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/back/state_machine.hpp>
#include <boost/mpl/vector.hpp>

#include "StartState.h"
#include "EhloState.h"
#include "MailFromState.h"
#include "RcptToState.h"
#include "DataState.h"
#include "QuitState.h"

namespace StubMTP {
namespace Smtp {

class Event
{
public:
    Event(const std::string & _data) : m_data(_data) { }
    const std::string & data() const { return m_data; }
private:
    std::string m_data;
}; // class Event

#define EVENT(name)                       \
    class name : public Event             \
    {                                     \
    public:                               \
        name(const std::string & _data) : \
            Event(_data)                  \
        {                                 \
        }                                 \
    };

EVENT(EhloEvent)
EVENT(MailFromEvent)
EVENT(RcptToEvent)
EVENT(DataEvent)
EVENT(QuitEvent)


class StateMachineDef :
    public boost::msm::front::state_machine_def<StateMachineDef>
{
public:
    typedef StartState initial_state;
    typedef StateBase  BaseAllStates;

    struct transition_table : boost::mpl::vector<
        // Start               Event          Next
        _row< StartState,    EhloEvent,     EhloState     >,
        _row< EhloState,     MailFromEvent, MailFromState >,
        _row< MailFromState, MailFromEvent, MailFromState >,
        _row< MailFromState, RcptToEvent,   RcptToState   >,
        _row< RcptToState,   RcptToEvent,   RcptToState   >,
        _row< RcptToState,   DataEvent,     DataState     >,
        _row< RcptToState,   QuitEvent,     QuitState     >
    > { };
}; // class StateMachineDef


typedef boost::msm::back::state_machine<StateMachineDef> StateMachine;


} // namespace Smtp
} // namespace StubMTP

#endif // __STUBMTP_SMTP_STATEMACHINE_STATEMACHINE_H__
