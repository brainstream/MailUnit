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

#include <MailUnitUI/Gui/QueryWidget.h>

using namespace MailUnit::Gui;

QueryWidget::QueryWidget(const ServerConfig & _server, QWidget * _parent /*= nullptr*/) :
    QWidget(_parent),
    m_server(_server),
    mp_notifier(nullptr),
    m_busy(false)
{
    setupUi(this);
    mp_notifier = new MqpClientNotifier(this);
    connect(mp_notifier, SIGNAL(headerReceived(quint32,quint32)), this, SLOT(onHeaderReceived(quint32,quint32)));
    connect(mp_notifier, SIGNAL(messageReceived(Message)), this, SLOT(onMessageReceived(Message)));
    connect(mp_notifier, SIGNAL(finished()), this, SLOT(onRequestFinished()));
}

void QueryWidget::execute()
{
    if(m_busy) return;
    m_busy = true;
    mp_edit_result->clear();
    sendMqpRequestAsync(m_server, mp_edit_query->toPlainText(), mp_notifier);
}

void QueryWidget::onHeaderReceived(quint32 _status, quint32 _count)
{
    QString text = QString("Status code: %1. Messages count: %2\n").arg(_status).arg(_count);
    mp_edit_result->appendPlainText(text);
}

void QueryWidget::onMessageReceived(const Message & _message)
{
    mp_edit_result->appendPlainText(_message.body);
    mp_edit_result->appendPlainText("____________________________________\n\n");
}

void QueryWidget::onRequestFinished()
{
    m_busy = false;
}


