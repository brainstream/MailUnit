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

#include <QList>
#include <QListView>
#include <MailUnitUI/MqpClient/MqpMessage.h>

namespace MailUnit {
namespace Gui {

class MessageListView : public QListView
{
    Q_OBJECT

public:
    explicit MessageListView(const QList<const MqpMessage *> & _messages, QWidget * _parent = nullptr);
    void sync();

protected slots:
    void currentChanged(const QModelIndex & _current, const QModelIndex & _previous) override;

signals:
    void messageSelected(const MqpMessage * _message);

private:
    const QList<const MqpMessage *> & mr_messages;
}; // class MessageListView

} // namespace Gui
} // namespace MailUnit
