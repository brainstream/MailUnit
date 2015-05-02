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
//#include <QtCore/QThreadPool>

using namespace MailUnit::Gui;

//namespace {

//class MqpRequestTask : public QRunnable
//{
//public:
//    MqpRequestTask(const MMqpEndpoint & _endpoint, const QString & _request) :
//        mp_receiver(new QueryWidgetMessageReceiver()),
//        m_endpoint(_endpoint),
//        m_request(_request)
//    {
//    }

//    ~MqpRequestTask() override
//    {
//        delete mp_receiver;
//    }

//    const QueryWidgetMessageReceiver & messageReceiver() const
//    {
//        return *mp_receiver;
//    }

//    void run();

//private:
//    static void onMessageReceived(MU_MQPMESSAGE _message, void *_task);

//private:
//    Q_DISABLE_COPY(MqpRequestTask)

//private:
//    QueryWidgetMessageReceiver * mp_receiver;
//    MMqpEndpoint m_endpoint;
//    QString m_request;
//}; // class MqpRequestTask

//} // namespace

//void MqpRequestTask::run()
//{
//    MMqpResponseHeader header = muMqpSendRequest(&m_endpoint, m_request.toUtf8(), onMessageReceived, this);
//    mp_receiver->sendHeader(header.status_code, header.afected_count);
//    mp_receiver->finish();
//}

//void MqpRequestTask::onMessageReceived(MU_MQPMESSAGE _message, void *_task)
//{
//    MqpRequestTask * self = static_cast<MqpRequestTask *>(_task);
//    self->mp_receiver->sendMessage("TEST MESSAGE");
//}

QueryWidget::QueryWidget(const ServerConfig & _server, QWidget * _parent /*= nullptr*/) :
    QWidget(_parent)
{
    mp_client = new MqpClient(_server, this);
    setupUi(this);
    connect(mp_client, SIGNAL(headerReceived(quint32,quint32)), this, SLOT(onHeaderReceived(quint32,quint32)));
    connect(mp_client, SIGNAL(messageReceived(Message)), this, SLOT(onMessageReceived(Message)));
    connect(mp_client, SIGNAL(finished()), this, SLOT(onRequestFinished()));
}

void QueryWidget::execute()
{
    if(mp_client->busy()) return;
    mp_edit_result->clear();
    mp_client->sendRequest(mp_edit_query->toPlainText());

//    MqpRequestTask * task = new MqpRequestTask(m_server_endpoint, mp_edit_query->toPlainText());
//    task->setAutoDelete(false);
//    mp_request_handler = task;
//    const QueryWidgetMessageReceiver * receiver = &task->messageReceiver();
//    connect(receiver, SIGNAL(headerReceived(quint32, quint32)), this, SLOT(onHeaderReceived(quint32, quint32)));
//    connect(receiver, SIGNAL(messageReceived(QString)), this, SLOT(onMessageReceived(QString)));
//    connect(receiver, SIGNAL(finished()), this, SLOT(onRequestFinished()));
//    QThreadPool::globalInstance()->start(task);
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
}


