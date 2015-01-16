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

#ifndef __MU_SMTP_STATEMACHINE_RCPTTOSTATE_H__
#define __MU_SMTP_STATEMACHINE_RCPTTOSTATE_H__

#include <MailUnit/Smtp/StateMachine/SingleLineCmdState.h>

namespace MailUnit {
namespace Smtp {


class RcptToState : public SingleLineCmdState
{
public:
    RcptToState();
    boost::optional<ResponseCode> processInput(const char * _data, Protocol & _protocol) override;
}; // class RcptToState


} // namespace Smtp
} // namespace MailUnit

#endif // __MU_SMTP_STATEMACHINE_RCPTTOSTATE_H__