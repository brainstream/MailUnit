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

#ifndef __STUBMTP_SMTP_STATEMACHINE_SINGLELINECMDSTATE_H__
#define __STUBMTP_SMTP_STATEMACHINE_SINGLELINECMDSTATE_H__

#include <StubMTP/Smtp/StateMachine/StartState.h>

namespace StubMTP {
namespace Smtp {


class SingleLineCmdState : public State
{
protected:
    enum class ProcessResult
    {
        Error,
        Success,
        Incomplete
    }; // enum class ProcessResult

protected:
    SingleLineCmdState(size_t args_pos);
    ProcessResult internalProcessInput(const std::string & _input);
    bool commandArgString(std::string & _result) const;
    ProcessResult currentState() const { return m_state; }
    void reset() override;

private:
    const size_t m_args_pos;
    std::string m_data;
    ProcessResult m_state;
}; // class SingleLineCmdState


} // namespace Smtp
} // namespace StubMTP

#endif // __STUBMTP_SMTP_STATEMACHINE_SINGLELINECMDSTATE_H__
