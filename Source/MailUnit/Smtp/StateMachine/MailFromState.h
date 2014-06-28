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

#ifndef __MU_SMTP_STATEMACHINE_MAILFROMSTATE_H__
#define __MU_SMTP_STATEMACHINE_MAILFROMSTATE_H__

#include <MailUnit/Smtp/StateMachine/SingleLineCmdState.h>

namespace MailUnit {
namespace Smtp {


class MailFromState : public SingleLineCmdState
{
public:
    MailFromState();

    void processInput(const std::string & _input, Message & _message)  override;

    bool isInputProcessingCompleted() const  override
    {
        return currentState() != ProcessResult::Incomplete;
    }

    bool isProtocolProcessingCompleted() const  override
    {
        return false;
    }

    bool response(ResponseCode * _response) const override;
}; // class MailFromState


} // namespace Smtp
} // namespace MailUnit

#endif // __MU_SMTP_STATEMACHINE_MAILFROMSTATE_H__
