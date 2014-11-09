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

#ifndef __MU_SMTP_RESPONSECODE_H__
#define __MU_SMTP_RESPONSECODE_H__

#include <string>

namespace MailUnit {
namespace Smtp {


enum class ResponseCode
{
    Status                         = 211,
    Help                           = 214,
    Ready                          = 220,
    Closing                        = 221,
    Ok                             = 250,
    Forward                        = 251,
    UserNotVerified                = 252,
    Intermediate                   = 354,
    ServiceNotAvailable            = 421,
    MailboxAnavailable             = 450,
    InternalError                  = 451,
    InsufficientSystemStorage      = 452,
    InvalidParameters              = 455,
    UnrecognizedCommand            = 500,
    UnrecognizedParameters         = 501,
    CommandNotImplemented          = 502,
    BadCommandsSequence            = 503,
    CommandParameterNotImplemented = 504,
    MailboxUnavailable             = 550,
    ForvardPath                    = 551,
    MailActionAborted              = 552,
    MailboxNameNotAllowed          = 553,
    TransactionFailed              = 554,
    MailAddressNotRecognized       = 555
}; // enum class ResponseCode


std::string translateResponseCode(ResponseCode _code);


} // namespace Smtp
} // namespace MailUnit

#endif // __MU_SMTP_RESPONSECODE_H__
