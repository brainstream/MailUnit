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

#include <MailUnit/Smtp/StateMachine/MailFromState.h>
#include <MailUnit/Smtp/ProtocolDef.h>

#define COMMAND SMTP_CMD_MAIL " FROM:"

using namespace MailUnit::Smtp;

MailFromState::MailFromState() :
    SingleLineCmdState(sizeof(COMMAND) - 1)
{
}

boost::optional<ResponseCode> MailFromState::processInput(const char * _data, Protocol & _protocol)
{
    if(!internalProcessInput(_data))
    {
        return nullptr;
    }
    std::string from = args();
    if(from.empty())
    {
        reset();
        throw StateException(ResponseCode::invalidParameters, "Address is required");
    }
    else
    {
        _protocol.email().addFromAddress(from);
        return ResponseCode::ok;
    }
}
