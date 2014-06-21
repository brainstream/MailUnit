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

#include <StubMTP/Server/Smtp/ResponseCode.h>
#include <StubMTP/Server/Smtp/ProtocolDef.h>

using namespace StubMTP::Smtp;

std::string StubMTP::Smtp::translateResponseCode(ResponseCode _code)
{
    switch(_code)
    {
    case ResponseCode::Ready:
        return "220 [127.0.0.1] Stub Mail Transfer Service Ready" SMTP_NEWLINE;
    case ResponseCode::Closing:
        return "221 [127.0.0.1] Stub Mail Transfer Service Closing" SMTP_NEWLINE;
    case ResponseCode::Ok:
        return "250 OK" SMTP_NEWLINE;
    case ResponseCode::Intermediate:
        return "354 Intermediate" SMTP_NEWLINE;
    case ResponseCode::BadCommandsSequence:
        return "503  Bad sequence of commands" SMTP_NEWLINE;
    case ResponseCode::InvalidParameters:
        return "455 Server unable to accommodate parameters" SMTP_NEWLINE;
    default:
        return "451 Internal server error" SMTP_NEWLINE;
    }
}
