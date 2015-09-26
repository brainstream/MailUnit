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

#include <MailUnitUI/MqpClient/MqpMessage.h>

using namespace MailUnit::Gui;

MqpMessage::MqpMessage(const MqpRawMessage & _raw_message) :
    mp_header(new MqpMessageHeader(_raw_message)),
    mp_mime_message(new MimeMessage(_raw_message.body))
{
}

MqpMessage::MqpMessage(const MqpMessage & _message) :
    mp_header(new MqpMessageHeader(*_message.mp_header)),
    mp_mime_message(new MimeMessage(*_message.mp_mime_message))
{
}

MqpMessage::MqpMessage(MqpMessage && _message) :
    mp_header(_message.mp_header),
    mp_mime_message(_message.mp_mime_message)
{
    _message.mp_header = nullptr;
    _message.mp_mime_message = nullptr;
}

MqpMessage::~MqpMessage()
{
    delete mp_header;
    delete mp_mime_message;
}

MqpMessage & MqpMessage::operator = (const MqpMessage & _message)
{
    if(this != &_message)
    {
        delete mp_header;
        delete mp_mime_message;
        mp_header = new MqpMessageHeader(*_message.mp_header);
        mp_mime_message = new MimeMessage(*_message.mp_mime_message);
    }
    return *this;
}

MqpMessage & MqpMessage::operator = (MqpMessage && _message)
{
    if(this != &_message)
    {
        delete mp_header;
        delete mp_mime_message;
        mp_header = new MqpMessageHeader(*_message.mp_header);
        mp_mime_message = new MimeMessage(*_message.mp_mime_message);
        _message.mp_header = nullptr;
        _message.mp_mime_message = nullptr;
    }
    return *this;
}

const MqpMessageHeader & MqpMessage::header() const
{
    return *mp_header;
}

const MimeMessage & MqpMessage::mime() const
{
    return *mp_mime_message;
}
