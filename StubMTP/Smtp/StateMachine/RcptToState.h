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

#ifndef __STUBMTP_SMTP_STATEMACHINE_RCPTTOSTATE_H__
#define __STUBMTP_SMTP_STATEMACHINE_RCPTTOSTATE_H__

#include <StubMTP/Smtp/StateMachine/SingleLineCmdState.h>

namespace StubMTP {
namespace Smtp {


class RcptToState : public SingleLineCmdState
{
public:
    RcptToState();

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
}; // class RcptToState


} // namespace Smtp
} // namespace StubMTP

#endif // __STUBMTP_SMTP_STATEMACHINE_RCPTTOSTATE_H__
