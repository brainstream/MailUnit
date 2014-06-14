/***********************************************************************************************
 *                                                                                             *
 * This file is part of StubMTP.                                                               *
 *                                                                                             *
 * StubMTP is free software: you can redistribute it and/or modify it under the terms of       *
 * the GNU General Public License as published by the Free Software Foundation,                *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * StubMTP is distributed in the hope that it will be useful,  but WITHOUT ANY WARRANTY;       *
 * without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  *
 * See the GNU General Public License for more details.                                        *
 *                                                                                             *
 * You should have received a copy of the GNU General Public License along with StubMTP.       *
 * If not, see <http://www.gnu.org/licenses/>.                                                 *
 *                                                                                             *
 ***********************************************************************************************/

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
