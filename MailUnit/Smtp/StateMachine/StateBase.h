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

#ifndef __MU_SMTP_STATEMACHINE_STATEBASE_H__
#define __MU_SMTP_STATEMACHINE_STATEBASE_H__

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/msm/front/states.hpp>
#include <MailUnit/Smtp/ResponseCode.h>
#include <MailUnit/Storage/Email.h>

namespace MailUnit {
namespace Smtp {

enum class StateStatus
{
    incompleted   = 0x1,
    completed     = 0x2,
    intermediate  = 0X4,
    emailReady    = 0x8,
    terminated    = 0x10
}; // enum class StateStatus


class StateBase : private boost::noncopyable
{
public:
    virtual ~StateBase() { }
    virtual void processInput(const std::string & _input, Storage::RawEmail & _email) = 0;
    virtual StateStatus response(ResponseCode & _response) const = 0;
    bool isInputProcessCompleted() const
    {
        ResponseCode resp;
        switch(response(resp))
        {
        case StateStatus::intermediate:
        case StateStatus::incompleted:
            return false;
        default:
            return true;
        }
    }
}; // class StateBase


class State : public boost::msm::front::state<StateBase>
{
public:
    template <typename EventT, typename FsmT>
    void on_entry(const EventT & _event, FsmT &)
    {
        processInput(_event.data(), _event.email());
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
} // namespace MailUnit

#endif // __MU_SMTP_STATEMACHINE_STATEBASE_H__
