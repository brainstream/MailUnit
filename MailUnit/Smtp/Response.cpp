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

#include <sstream>
#include <boost/preprocessor/stringize.hpp>
#include <boost/asio/ip/host_name.hpp>
#include <MailUnit/Smtp/Response.h>

using namespace MailUnit::Smtp;

namespace {

std::string greeting()
{
    std::stringstream result;
    try
    {
        result << boost::asio::ip::host_name();
    }
    catch(...)
    {
        result << "unknown_host";
    }
    result << ' ' << BOOST_PP_STRINGIZE(_MU_SERVER_NAME);
    return result.str();
}

} // namespace

std::string MailUnit::Smtp::responseCodeDefaultMessage(ResponseCode _code)
{
    switch(_code)
    {
    case ResponseCode::status:
        return "";
    case ResponseCode::help:
        return "";
    case ResponseCode::ready:
        return greeting();
    case ResponseCode::closing:
        return "Closing";
    case ResponseCode::ok:
        return "OK";
    case ResponseCode::forward:
        return "";
    case ResponseCode::userNotVerified:
        return "User is not verified";
    case ResponseCode::intermediate:
        return "Intermediate";
    case ResponseCode::serviceNotAvailable:
        return "Service in not available";
    case ResponseCode::mailboxBusy:
        return "Mailbox busy";
    case ResponseCode::internalError:
        return "Internal server error";
    case ResponseCode::insufficientSystemStorage:
        return "Insufficient system storage";
    case ResponseCode::invalidParameters:
        return "Invalid parameters";
    case ResponseCode::unrecognizedCommand:
        return "Unrecognized command";
    case ResponseCode::unrecognizedParameters:
        return "Unrecognized parameters";
    case ResponseCode::commandNotImplemented:
        return "Command is not implemented";
    case ResponseCode::badCommandsSequence:
        return "Bad commands sequence";
    case ResponseCode::commandParameterNotImplemented:
        return "Command parameter is not implemented";
    case ResponseCode::mailboxUnavailable:
        return "Mailbox unavailable";
    case ResponseCode::forwardPath:
        return "";
    case ResponseCode::mailActionAborted:
        return "Action aborted";
    case ResponseCode::mailboxNameNotAllowed:
        return "Invalid mailbox name";
    case ResponseCode::transactionFailed:
        return "Transaction failed";
    case ResponseCode::mailAddressNotRecognized:
        return "Mail address is not recognized";
    default:
        return "";
    }
}

void Response::addExtenstion(const ProtocolExtenstion & _extension)
{
    std::stringstream strstream;
    strstream << _extension;
    m_extenstions.push_back(strstream.str());
}

void Response::print(std::ostream & _stream) const
{
    if(m_extenstions.empty())
    {
        _stream << static_cast<short>(m_code) << ' ' << m_message;
    }
    else
    {
        _stream << static_cast<short>(m_code) << '-' << m_message;
        std::size_t len = m_extenstions.size();
        for(std::size_t i = 0; i < len - 1; ++i)
        {
            _stream << static_cast<short>(m_code) << '-' << m_extenstions[i];
        }
        _stream << static_cast<short>(m_code) << ' ' << m_extenstions.back();
    }
}
