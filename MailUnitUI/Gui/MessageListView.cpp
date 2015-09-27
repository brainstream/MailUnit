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

#include <QTextStream>
#include <MailUnitUI/Gui/MessageListView.h>

using namespace MailUnit::Gui;

class MessageListViewModel : public QAbstractListModel
{
public:
    explicit MessageListViewModel(const QList<const MqpMessage *> & _messages, QObject * _parent = nullptr) :
        QAbstractListModel(_parent),
        m_row_count(0),
        mr_messages(_messages)
    {
    }

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex & _index, int _role) const;
    void sync();

private:
    QString formatMessage(const MqpMessage & _message) const;

private:
    int m_row_count;
    const QList<const MqpMessage *> & mr_messages;
}; // class MessageListViewModel

int MessageListViewModel::rowCount(const QModelIndex &parent) const
{
   return parent.isValid() ? 0 : m_row_count;
}

QVariant MessageListViewModel::data(const QModelIndex & _index, int _role) const
{
    if(!_index.isValid())
    {
        return QVariant();
    }
    switch(_role)
    {
    case Qt::DisplayRole:
        return formatMessage(*mr_messages[_index.row()]);
    default:
        return QVariant();
    }

}

QString MessageListViewModel::formatMessage(const MqpMessage & _message) const
{
    QString string;
    QTextStream stream(&string, QIODevice::WriteOnly);
    const MqpMessageHeader & header = _message.header();
    if(header.subject.isEmpty())
    {
        stream << tr("<NO SUBJECT>");
    }
    else
    {
        stream << header.subject;
    }
    if(header.from.length() > 0)
    {
        stream << "\nFrom: " << header.from.join(", ");
    }
    if(header.to.length() > 0)
    {
        stream << "\nTo: " << header.to.join(", ");
    }
    if(header.cc.length() > 0)
    {
        stream << "\nCC: " << header.cc.join(", ");
    }
    if(header.bcc.length() > 0)
    {
        stream << "\nBCC: " << header.bcc.join(", ");
    }
    return *stream.string();
}

void MessageListViewModel::sync()
{
    int old_row_count = m_row_count;
    m_row_count = mr_messages.length();
    emit dataChanged(createIndex(old_row_count, 0), createIndex(m_row_count, 0));
}

MessageListView::MessageListView(const QList<const MqpMessage *> & _messages, QWidget * _parent /*= nullptr*/) :
    QListView(_parent),
    mr_messages(_messages)
{
    delete model();
    setModel(new MessageListViewModel(_messages, this));
    setStyleSheet("::item { padding: 0.5em 0.2em; }");
    setAlternatingRowColors(true);
}

void MessageListView::sync()
{
    static_cast<MessageListViewModel *>(model())->sync();
}

void MessageListView::currentChanged(const QModelIndex & _current, const QModelIndex & _previous)
{
    QListView::currentChanged(_current, _previous);
    const MqpMessage * message = nullptr;
    if(_current.row() < mr_messages.length())
        message = mr_messages[_current.row()];
    emit messageSelected(message);
}
