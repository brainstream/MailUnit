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

#include <StubMTP/Server/Smtp/StateMachine/MailFromState.h>
#include <StubMTP/Server/Smtp/ProtocolDef.h>

#define COMMAND SMTP_CMD_MAIL " FROM:"

using namespace StubMTP::Smtp;

MailFromState::MailFromState() :
    SingleLineCmdState(sizeof(COMMAND) - 1)
{
}

void MailFromState::processInput(const std::string & _input, Message & _message)
{
    if(internalProcessInput(_input) == ProcessResult::Success)
        commandArgString(_message.from);
}

bool MailFromState::response(ResponseCode * _response) const
{
    switch(currentState())
    {
    case ProcessResult::Incomplete:
        return false;
    case ProcessResult::Error:
        *_response = ResponseCode::InvalidParameters;
        return true;
    default:
        *_response = ResponseCode::Ok;
        return true;
    }
}
