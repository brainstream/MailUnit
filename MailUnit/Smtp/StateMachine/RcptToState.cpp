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

#include <MailUnit/Smtp/StateMachine/RcptToState.h>
#include <MailUnit/Smtp/ProtocolDef.h>

#define COMMAND SMTP_CMD_RCPT " TO:"

using namespace MailUnit::Smtp;

RcptToState::RcptToState() :
    SingleLineCmdState(sizeof(COMMAND) - 1)
{
}

void RcptToState::processInput(const std::string & _input, Storage::RawEmail & _email)
{
    if(internalProcessInput(_input) != ProcessResult::success)
        return;
    std::string to;
    if(commandArgString(to))
        _email.addToAddress(to);
}

StateStatus RcptToState::response(ResponseCode & _response) const
{
    switch(currentState())
    {
    case ProcessResult::incompleted:
        return StateStatus::incompleted;
    case ProcessResult::error:
        _response = ResponseCode::invalidParameters;
        return StateStatus::completed;
    default:
        _response = ResponseCode::ok;
        return StateStatus::completed;
    }
}
