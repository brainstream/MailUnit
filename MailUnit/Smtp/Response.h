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

#ifndef __MU_SMTP_RESPONSE_H__
#define __MU_SMTP_RESPONSE_H__

#include <vector>
#include <sstream>
#include <MailUnit/Smtp/ProtocolExtension.h>

namespace MailUnit {
namespace Smtp {

enum class ResponseCode : short
{
    status                         = 211,
    help                           = 214,
    ready                          = 220,
    closing                        = 221,
    ok                             = 250,
    forward                        = 251,
    userNotVerified                = 252,
    intermediate                   = 354,
    serviceNotAvailable            = 421,
    mailboxBusy                    = 450,
    internalError                  = 451,
    insufficientSystemStorage      = 452,
    invalidParameters              = 455,
    unrecognizedCommand            = 500,
    unrecognizedParameters         = 501,
    commandNotImplemented          = 502,
    badCommandsSequence            = 503,
    commandParameterNotImplemented = 504,
    mailboxUnavailable             = 550,
    forwardPath                    = 551,
    mailActionAborted              = 552,
    mailboxNameNotAllowed          = 553,
    transactionFailed              = 554,
    mailAddressNotRecognized       = 555
}; // enum class ResponseCode

std::string responseCodeDefaultMessage(ResponseCode _code);

class Response
{
public:
    Response(ResponseCode _code) :
        Response(_code, responseCodeDefaultMessage(_code))
    {
    }

    Response(ResponseCode _code, const std::string & _message) :
        m_code(_code),
        m_message(_message)
    {
    }

    Response(const Response &) = default;
    Response & operator = (const Response &) = default;
    void addExtenstion(const ProtocolExtenstion & _extension);
    void print(std::ostream & _stream) const;

private:
    ResponseCode m_code;
    const std::string m_message;
    std::vector<std::string> m_extenstions;
}; // class Response

} // namespace Smtp
} // namespace MailUnit

inline std::ostream & operator << (std::ostream & _stream, const MailUnit::Smtp::Response & _response)
{
    _response.print(_stream);
    return _stream;
}

#endif // __MU_SMTP_RESPONSE_H__
