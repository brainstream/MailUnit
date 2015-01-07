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

#ifndef __MU_SMTP_STATEMACHINE_EVENT_H__
#define __MU_SMTP_STATEMACHINE_EVENT_H__

#include <MailUnit/Smtp/Protocol.h>

namespace MailUnit {
namespace Smtp {

template<unsigned int index>
class Event
{
}; // class Event

typedef Event<0> ErrorEvent;
typedef Event<1> EhloEvent;
typedef Event<2> MailFromEvent;
typedef Event<3> RcptToEvent;
typedef Event<4> DataHeaderEvent;
typedef Event<5> DataEvent;
typedef Event<6> QuitEvent;

} // namespace Smtp
} // namespace MailUnit

#endif // __MU_SMTP_STATEMACHINE_EVENT_H__
