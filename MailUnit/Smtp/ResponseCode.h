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
    status                         = 211,
    help                           = 214,
    ready                          = 220,
    closing                        = 221,
    ok                             = 250,
    forward                        = 251,
    userNotVerified                = 252,
    intermediate                   = 354,
    serviceNotAvailable            = 421,
    mailboxAnavailable             = 450,
    internalError                  = 451,
    insufficientSystemStorage      = 452,
    invalidParameters              = 455,
    unrecognizedCommand            = 500,
    unrecognizedParameters         = 501,
    commandNotImplemented          = 502,
    badCommandsSequence            = 503,
    commandParameterNotImplemented = 504,
    mailboxUnavailable             = 550,
    forvardPath                    = 551,
    mailActionAborted              = 552,
    mailboxNameNotAllowed          = 553,
    transactionFailed              = 554,
    mailAddressNotRecognized       = 555
}; // enum class ResponseCode


std::string translateResponseCode(ResponseCode _code);


} // namespace Smtp
} // namespace MailUnit

#endif // __MU_SMTP_RESPONSECODE_H__
