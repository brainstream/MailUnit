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
#include <MailUnit/Smtp/StateMachine/Event.h>
#include <MailUnit/Smtp/StateMachine/StartState.h>
#include <MailUnit/Smtp/StateMachine/EhloState.h>
#include <MailUnit/Smtp/StateMachine/MailFromState.h>
#include <MailUnit/Smtp/StateMachine/InvalidMailFromState.h>
#include <MailUnit/Smtp/StateMachine/RcptToState.h>
#include <MailUnit/Smtp/StateMachine/InvalidRcptToState.h>
#include <MailUnit/Smtp/StateMachine/DataHeaderState.h>
#include <MailUnit/Smtp/StateMachine/DataState.h>
#include <MailUnit/Smtp/StateMachine/QuitState.h>

namespace MailUnit {
namespace Smtp {

class StateMachineDef :
    public boost::msm::front::state_machine_def<StateMachineDef>
{
public:
    typedef StartState initial_state;
    typedef StateBase  BaseAllStates;

    struct transition_table : boost::mpl::vector<
        //    Start                   Event               Next
        _row< StartState,             EhloEvent,          EhloState            >,
        _row< StartState,             QuitEvent,          QuitState            >,
        _row< EhloState,              MailFromEvent,      MailFromState        >,
        _row< EhloState,              QuitEvent,          QuitState            >,
        _row< MailFromState,          ErrorEvent,         InvalidMailFromState >,
        _row< InvalidMailFromState,   MailFromEvent,      MailFromState        >,
        _row< MailFromState,          MailFromEvent,      MailFromState        >,
        _row< MailFromState,          RcptToEvent,        RcptToState          >,
        _row< MailFromState,          QuitEvent,          QuitState            >,
        _row< RcptToState,            RcptToEvent,        RcptToState          >,
        _row< RcptToState,            DataHeaderEvent,    DataHeaderState      >,
        _row< RcptToState,            QuitEvent,          QuitState            >,
        _row< RcptToState,            ErrorEvent,         InvalidRcptToState   >,
        _row< InvalidRcptToState,     RcptToEvent,        RcptToState          >,
        _row< DataHeaderState,        DataEvent,          DataState            >,
        _row< DataState,              QuitEvent,          QuitState            >,
        _row< DataState,              MailFromEvent,      MailFromState        >
    > { };
}; // class StateMachineDef

class StateMachine : public boost::msm::back::state_machine<StateMachineDef>
{
public:
    StateBase * currentState()
    {
        return get_state_by_id(current_state()[0]);
    }
}; // class StateMachine

} // namespace Smtp
} // namespace MailUnit

#endif // __MU_SMTP_STATEMACHINE_STATEMACHINE_H__
