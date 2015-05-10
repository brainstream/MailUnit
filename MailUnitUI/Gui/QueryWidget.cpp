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
    mp_state(nullptr)
{
    setupUi(this);
    mp_label_server->setText(QString("%1 [%2:%3]")
        .arg(_server.name())
        .arg(_server.host())
        .arg(_server.port()));
    mp_label_status->setText(tr("Ready"));
    mp_progress_bar->setVisible(false);
    mp_notifier = new MqpClientNotifier(this);
    connect(mp_notifier, SIGNAL(headerReceived(MqpResponseHeader)), this, SLOT(onHeaderReceived(MqpResponseHeader)));
    connect(mp_notifier, SIGNAL(messageReceived(Message)), this, SLOT(onMessageReceived(Message)));
    connect(mp_notifier, SIGNAL(finished()), this, SLOT(onRequestFinished()));
}

QueryWidget::~QueryWidget()
{
    delete mp_state;
}

void QueryWidget::execute()
{
    if(mp_state) return;
    mp_state = new LoadingState({ });
    mp_edit_result->clear();
    mp_progress_bar->setRange(0, 0);
    mp_label_status->setVisible(false);
    mp_progress_bar->setVisible(true);
    sendMqpRequestAsync(m_server, mp_edit_query->toPlainText(), mp_notifier);
}

void QueryWidget::onHeaderReceived(const MqpResponseHeader & _header)
{
    mp_state->header = _header;
    mp_state->loaded_count = 0;
    mp_progress_bar->setRange(0, static_cast<int>(_header.affected_count));
    mp_progress_bar->setValue(0);
}

void QueryWidget::onMessageReceived(const Message & _message)
{
    ++mp_state->loaded_count;
    mp_progress_bar->setValue(static_cast<int>(mp_state->loaded_count));
    mp_edit_result->appendPlainText(_message.body);
    mp_edit_result->appendPlainText("____________________________________\n\n");
}

void QueryWidget::onRequestFinished()
{
    switch(mp_state->header.response_type)
    {
    case MqpResponseType::matched:
        mp_label_status->setText(tr("Loaded %1 message(s)").arg(mp_state->loaded_count));
        break;
    case MqpResponseType::deleted:
        mp_label_status->setText(tr("Deleted %1 message(s)").arg(mp_state->loaded_count));
        break;
    default:
        mp_label_status->setText(tr("Error: %1").arg(mp_state->header.status_code));
        break;
    }
    mp_progress_bar->setVisible(false);
    mp_label_status->setVisible(true);
    delete mp_state;
    mp_state = nullptr;
}


