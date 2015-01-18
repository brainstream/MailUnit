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

#ifndef __MU_SMTP_STATEMACHINE_READYSTATE_H__
#define __MU_SMTP_STATEMACHINE_READYSTATE_H__

#include <MailUnit/Smtp/StateMachine/StateBase.h>

namespace MailUnit {
namespace Smtp {

class ReadyState : public State
{
public:
    boost::optional<ResponseCode> processInput(const char *, Protocol &) override
    {
        return ResponseCode::ready;
    }

protected:
    void reset() override
    {
    }
}; // class ReadyState

} // namespace Smtp
} // namespace MailUnit

#endif // __MU_SMTP_STATEMACHINE_READYSTATE_H__
