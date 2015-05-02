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

#ifndef __MUGUI_MESSAGE_H__
#define __MUGUI_MESSAGE_H__

#include <QtCore/QMetaType>
#include <QtCore/QString>

namespace MailUnit {
namespace Gui {

struct Message
{
    Message()
    {
        static bool registered = false;
        if(!registered)
        {
            registered = true;
            qRegisterMetaType<Message>("Message");
        }
    }

    QString subject;
    QByteArray body;
}; // struct Message

} // namespace Gui
} // namespace MailUnit

#endif // __MUGUI_MESSAGE_H__
