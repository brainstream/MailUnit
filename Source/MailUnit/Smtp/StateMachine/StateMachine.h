/***********************************************************************************************
 *                                                                                             *
 * This file is part of MailUnit.                                                              *
 *                                                                                             *
 * MailUnit is free software: you can redistribute it and/or modify it under the terms of      *
 * the GNU General Public License as published by the Free Software Foundation,                *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * MailUnit is distributed in the hope that it will be useful,  but WITHOUT ANY WARRANTY;      *
 * without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  *
 * See the GNU General Public License for more details.                                        *
 *                                                                                             *
 * You should have received a copy of the GNU General Public License along with MailUnit.      *
 * If not, see <http://www.gnu.org/licenses/>.                                                 *
 *                                                                                             *
 ***********************************************************************************************/

#ifndef __MU_SMTP_STATEMACHINE_STATEMACHINE_H__
#define __MU_SMTP_STATEMACHINE_STATEMACHINE_H__

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/back/state_machine.hpp>
#include <boost/mpl/vector.hpp>
#include <MailUnit/Smtp/StateMachine/StartState.h>
#include <MailUnit/Smtp/StateMachine/EhloState.h>
#include <MailUnit/Smtp/StateMachine/MailFromState.h>
#include <MailUnit/Smtp/StateMachine/RcptToState.h>
#include <MailUnit/Smtp/StateMachine/DataState.h>
#include <MailUnit/Smtp/StateMachine/QuitState.h>

namespace MailUnit {
namespace Smtp {

class Event
{
public:
    Event(const std::string & _data, Message & _message) :
        m_data(_data),
        mr_message(_message)
    {
    }

    const std::string & data() const
    {
        return m_data;
    }

    Message & message() const
    {
        return mr_message;
    }

private:
    std::string m_data;
    Message & mr_message;
}; // class Event

#define EVENT(name)                                           \
    class name : public Event                                 \
    {                                                         \
    public:                                                   \
        name(const std::string & _data, Message & _message) : \
            Event(_data, _message)                            \
        {                                                     \
        }                                                     \
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
        _row< RcptToState,   QuitEvent,     QuitState     >,
        _row< DataState,     QuitEvent,     QuitState     >
    > { };
}; // class StateMachineDef


typedef boost::msm::back::state_machine<StateMachineDef> StateMachine;


} // namespace Smtp
} // namespace MailUnit

#endif // __MU_SMTP_STATEMACHINE_STATEMACHINE_H__
