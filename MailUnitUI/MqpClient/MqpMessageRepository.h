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

#ifndef __MUGUI_MQPCLIENT_MQPMESSAGEREPOSITORY_H__
#define __MUGUI_MQPCLIENT_MQPMESSAGEREPOSITORY_H__

#include <memory>
#include <vector>
#include <QObject>
#include <MailUnitUI/MqpClient/MqpMessage.h>

namespace MailUnit {
namespace Gui {

class MqpMessageRepository : public QObject
{
    Q_OBJECT

public:
    explicit MqpMessageRepository(QObject * _parent = nullptr) :
        QObject(_parent)
    {
    }

    void appendMessage(std::unique_ptr<MqpRawMessage> _message)
    {
        size_t index = m_messages.size();
        m_messages.push_back(std::move(_message));
        emit messageReceived(index);
    }

    const MqpRawMessage & get(size_t _index) const
    {
        return *m_messages[_index];
    }

    size_t count() const
    {
        return m_messages.size();
    }

signals:
    void messageReceived(size_t _index);

private:
    std::vector<std::unique_ptr<MqpRawMessage>> m_messages;
}; // class MqpMessageRepository

} // namespace Gui
} // namespace MailUnit

#endif // __MUGUI_MQPCLIENT_MQPMESSAGEREPOSITORY_H__
