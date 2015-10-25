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

#include <QNetworkReply>
#include <MailUnitUI/Gui/QueryWidget.h>
#include <LibMailUnit/Message/Mime.h>

using namespace MailUnit::Gui;

namespace {

class MimeContent : public QNetworkReply
{
public:
    explicit MimeContent(QObject * _parent = nullptr);
    bool isSequential() const;
    qint64 bytesAvailable() const;

protected:
    qint64 readData(char * _data, qint64 _maxlen);

public slots:
    void abort();
}; // class MimeContent

class NetworkAccessManager : public QNetworkAccessManager
{
public:
    NetworkAccessManager(QNetworkAccessManager & _default_manager, QObject * _parent = nullptr);

protected:
    QNetworkReply * createRequest(Operation _op, const QNetworkRequest & _request, QIODevice * _outgoing_data);

private:
    QNetworkAccessManager & mr_default;
}; // class NetworkAccessManager

} // namespace

MimeContent::MimeContent(QObject * _parent /*= nullptr*/) :
    QNetworkReply(_parent)
{
}

bool MimeContent::isSequential() const
{
    return false;
}

qint64 MimeContent::bytesAvailable() const
{
    return 0;
}

qint64 MimeContent::readData(char * _data, qint64 _maxlen)
{
    return 0;
}

void MimeContent::abort()
{
}

NetworkAccessManager::NetworkAccessManager(QNetworkAccessManager & _default_manager, QObject * _parent /*= nullptr*/) :
    QNetworkAccessManager(_parent),
    mr_default(_default_manager)
{
    // TODO: accept a MU_MIME_PART
}

QNetworkReply * NetworkAccessManager::createRequest(Operation _op, const QNetworkRequest & _request, QIODevice * _outgoing_data)
{
    if(_request.url().scheme().compare("cit", Qt::CaseInsensitive) != 0)
        return QNetworkAccessManager::createRequest(_op, _request, _outgoing_data);
    return new MimeContent(this);
}

QueryWidget::QueryWidget(const ServerConfig & _server, QWidget * _parent /*= nullptr*/) :
    QWidget(_parent),
    mp_client(nullptr),
    m_server(_server),
    mp_state(nullptr),
    mp_messages(new QList<const MqpMessage *>),
    mp_html_view(nullptr)
{
    setupUi(this);
    mp_client = new MqpClient(m_server, this);
    mp_listview_result = new MessageListView(*mp_messages, this);
    mp_listview_result->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    mp_layout_result->insertWidget(0, mp_listview_result);
    mp_label_server->setText(QString("%1 [%2:%3]")
        .arg(_server.name())
        .arg(_server.host())
        .arg(_server.port()));
    mp_label_status->setText(tr("Ready"));
    mp_progress_bar->setVisible(false);
    mp_html_view = new HtmlView(this);
    mp_tab_html->layout()->addWidget(mp_html_view);
    connect(mp_client, SIGNAL(connected(MqpResponseHeader)), this, SLOT(onClientConnected(MqpResponseHeader)));
    connect(mp_client, SIGNAL(finished()), this, SLOT(onRequestFinished()));
    connect(mp_client, SIGNAL(messageReceived(MqpRawMessage)), this, SLOT(onMessageReceived(MqpRawMessage)));
    connect(mp_listview_result, SIGNAL(messageSelected(const MqpMessage*)), this, SLOT(onMessageSelected(const MqpMessage*)));
}

QueryWidget::~QueryWidget()
{
    delete mp_state;
    for(const MqpMessage * msg : *mp_messages)
        delete msg;
    delete mp_messages;
}

void QueryWidget::execute()
{
    if(mp_state) return;
    mp_state = new LoadingState({ });
    mp_messages->clear();
    mp_listview_result->sync();
    mp_edit_msg_body->clear();
    mp_progress_bar->setRange(0, 0);
    mp_label_status->setVisible(false);
    mp_progress_bar->setVisible(true);
    mp_client->executeRequest(mp_edit_query->toPlainText());
}

void QueryWidget::onClientConnected(const MqpResponseHeader & _header)
{
    mp_state->header = _header;
    mp_state->loaded_count = 0;
    mp_progress_bar->setRange(0, static_cast<int>(_header.affected_count));
    mp_progress_bar->setValue(0);
}

void QueryWidget::onMessageReceived(const MqpRawMessage & _message)
{
    ++mp_state->loaded_count;
    mp_progress_bar->setValue(static_cast<int>(mp_state->loaded_count));
    mp_messages->append(new MqpMessage(_message));
    mp_listview_result->sync();
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

void QueryWidget::onMessageSelected(const MqpMessage * _message)
{
    mp_edit_msg_body->clear();
    mp_html_view->clear();
    if(nullptr == _message)
        return;
    setPlainTextContent(_message->mime());
    setHtmlContent(_message->mime());
}

void QueryWidget::setPlainTextContent(const MimeMessage & _message)
{
    if(_message.contentType() == "text" && _message.contentSubtype() == "plain")
    {
        mp_edit_msg_body->setPlainText(_message.content());
    }
    else if(_message.contentType() == "multipart")
    {
        for(const std::unique_ptr<const MimeMessagePart> & part : _message.parts())
        {
            // TODO: recursive
            if(part->contentType() == "text" && part->contentSubtype() == "plain")
            {
                mp_edit_msg_body->setPlainText(part->content());
            }
        }
    }
}

void QueryWidget::setHtmlContent(const MimeMessage & _message)
{
    if(_message.contentType() == "text" && _message.contentSubtype() == "html")
    {
        mp_html_view->setSource(_message);
    }
    else if(_message.contentType() == "multipart")
    {
        for(const std::unique_ptr<const MimeMessagePart> & part : _message.parts())
        {
            // TODO: recursive
            if(part->contentType() == "text" && part->contentSubtype() == "html")
            {
                mp_html_view->setSource(*part);
            }
        }
    }
}
