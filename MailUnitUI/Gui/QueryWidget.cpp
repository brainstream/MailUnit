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
    mp_mqp_client(nullptr)
{
    setupUi(this);
    mp_mqp_client = new MqpClient(_server, this);
    connect(mp_mqp_client, SIGNAL(messageReceived(QString)), this, SLOT(onMessageReceived(QString)));
}

void QueryWidget::execute()
{
    mp_edit_result->clear();
    mp_mqp_client->query(mp_edit_query->toPlainText());
}

void QueryWidget::onMessageReceived(const QString & _data)
{
    mp_edit_result->appendPlainText(_data);
}
