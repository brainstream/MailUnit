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

#ifndef __STUBMTP_SMTP_STATEMACHINE_DATASTATE_H__
#define __STUBMTP_SMTP_STATEMACHINE_DATASTATE_H__

#include <StubMTP/Smtp/StateMachine/StartState.h>

namespace StubMTP {
namespace Smtp {


class DataState : public State
{
public:
    DataState();

    void processInput(const std::string & _input, Message & _message)  override;

    bool isInutProcessingCompleted() const  override
    {
        return m_complete;
    }

    bool isProtocolProcessingCompleted() const  override
    {
        return false;
    }

    bool response(ResponseCode * _response) const override;
    void reset() override;

private:
    void processHeader(const std::string & _input);
    void processData(const std::string & _input, Message & _message);

private:
    bool m_header_accepted;
    bool m_complete;
    bool m_error;
    std::string m_data;
}; // class DataState


} // namespace Smtp
} // namespace StubMTP

#endif // __STUBMTP_SMTP_STATEMACHINE_DATASTATE_H__
