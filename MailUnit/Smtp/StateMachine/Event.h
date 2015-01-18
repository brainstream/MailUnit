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

enum class EventId
{
    Error,
    Init,
    Ehlo,
    MailFrom,
    RcptTo,
    DataHeader,
    Data,
    Quit,
}; // enum class EventId

template<EventId id>
class Event
{
}; // class Event

typedef Event<EventId::Error> ErrorEvent;
typedef Event<EventId::Init> InitEvent;
typedef Event<EventId::Ehlo> EhloEvent;
typedef Event<EventId::MailFrom> MailFromEvent;
typedef Event<EventId::RcptTo> RcptToEvent;
typedef Event<EventId::DataHeader> DataHeaderEvent;
typedef Event<EventId::Data> DataEvent;
typedef Event<EventId::Quit> QuitEvent;

} // namespace Smtp
} // namespace MailUnit

#endif // __MU_SMTP_STATEMACHINE_EVENT_H__
