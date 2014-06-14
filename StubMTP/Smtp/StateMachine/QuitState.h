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

#ifndef __STUBMTP_SMTP_STATEMACHINE_QUITSTATE_H__
#define __STUBMTP_SMTP_STATEMACHINE_QUITSTATE_H__

#include <StubMTP/Smtp/StateMachine/StateBase.h>

namespace StubMTP {
namespace Smtp {


class QuitState : public State
{
public:
    void processInput(const std::string & _input, Message & _message)  override
    {
        STUBMTP_UNUSED(_input);
        STUBMTP_UNUSED(_message);
    }

    bool isInutProcessingCompleted() const  override
    {
        return true;
    }

    bool isProtocolProcessingCompleted() const  override
    {
        return true;
    }

    bool response(ResponseCode * _response) const override
    {
        *_response = ResponseCode::Closing;
        return true;
    }

    void reset() override
    {
    }
}; // class QuitState


} // namespace Smtp
} // namespace StubMTP

#endif // __STUBMTP_SMTP_STATEMACHINE_QUITSTATE_H__
