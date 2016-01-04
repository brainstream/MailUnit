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

#ifndef __MUGUI_MQPCLIENT_MQPRESPONSE_H__
#define __MUGUI_MQPCLIENT_MQPRESPONSE_H__

#include <QString>
#include <QStringList>
#include <MailUnitUI/MqpClient/MimeMessage.h>

namespace MailUnit {
namespace Gui {

struct MqpMessageHeader
{
    MqpMessageHeader()
    {
        static bool registered = false;
        if(!registered)
        {
            registered = true;
            qRegisterMetaType<MqpMessageHeader>("MqpMessageHeader");
        }
    }

    quint32 id;
    QStringList from;
    QStringList to;
    QStringList cc;
    QStringList bcc;
    QString subject;
}; // struct MqpMessageHeader

struct MqpRawMessage : MqpMessageHeader
{
    MqpRawMessage()
    {
        static bool registered = false;
        if(!registered)
        {
            registered = true;
            qRegisterMetaType<MqpRawMessage>("MqpRawMessage");
        }
    }

    QByteArray body;
}; // class MqpRawMessage

class MqpMessage final
{
public:
    MqpMessage(const MqpRawMessage & _raw_message);
    MqpMessage(const MqpMessage & _message);
    MqpMessage(MqpMessage && _message);
    ~MqpMessage();
    MqpMessage & operator = (const MqpMessage & _message);
    MqpMessage & operator = (MqpMessage && _message);
    const MqpMessageHeader & header() const;
    const MimeMessage & mime() const;

private:
    MqpMessageHeader * mp_header;
    MimeMessage * mp_mime_message;
}; // class MqpMessage

} // namespace Gui
} // namespace MailUnit

#endif // __MUGUI_MQPCLIENT_MQPRESPONSE_H__
