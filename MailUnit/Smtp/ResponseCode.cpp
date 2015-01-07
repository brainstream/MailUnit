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

#include <MailUnit/Smtp/ResponseCode.h>
#include <MailUnit/Smtp/ProtocolDef.h>

using namespace MailUnit::Smtp;

std::string MailUnit::Smtp::translateResponseCode(ResponseCode _code)
{
    switch(_code)
    {
    case ResponseCode::ready:
        return "220 MailUnit Ready" SMTP_NEWLINE;
    case ResponseCode::closing:
        return "221 MailUnit Closing" SMTP_NEWLINE;
    case ResponseCode::ok:
        return "250 OK" SMTP_NEWLINE;
    case ResponseCode::intermediate:
        return "354 Intermediate" SMTP_NEWLINE;
    case ResponseCode::badCommandsSequence:
        return "503 Bad sequence of commands" SMTP_NEWLINE;
    case ResponseCode::invalidParameters:
        return "455 Server unable to accommodate parameters" SMTP_NEWLINE;
    case ResponseCode::unrecognizedCommand:
        return "500 Unrecognized command" SMTP_NEWLINE;
    default:
        return "451 Internal server error" SMTP_NEWLINE;
    }
}
