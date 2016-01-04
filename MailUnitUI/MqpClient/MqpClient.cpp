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

#include <MailUnitUI/MqpClient/MqpClient.h>

using namespace MailUnit::Gui;

MqpClient::MqpClient(const ServerConfig & _server, QObject * _parent /*= nullptr*/) :
    QObject(_parent),
    m_server(_server)
{
    mp_client = muMqpCreateClient(_server.host().toLatin1(), _server.port());
}

MqpClient::~MqpClient()
{
    muFree(mp_command);
    muFree(mp_client);
}

void MqpClient::executeRequest(const QString & _request)
{
    muMqpSendCommand(mp_client, _request.toLatin1(), &mqpProc, this);
}

void MqpClient::mqpProc(MU_MqpEvent _event, const void * _arg, void * _user_data)
{
    MqpClient * client = static_cast<MqpClient *>(_user_data);
    switch(_event)
    {
    case mu_mqp_evt_error:
        client->onError(*static_cast<const unsigned int *>(_arg));
        break;
    case mu_mqp_evt_response_header:
        client->onResponseHeaderReceived(static_cast<const MU_MqpResponseHeader *>(_arg));
        break;
    case mu_mqp_evt_message:
        client->onMessageReceived(static_cast<const MU_MqpMessage *>(_arg));
        break;
    case mu_mqp_evt_finish:
        client->onFinished();
        break;
    }
}

void MqpClient::onResponseHeaderReceived(const MU_MqpResponseHeader * _header)
{
    MqpResponseHeader qheader;
    qheader.affected_count = _header->affected_count;
    qheader.status_code = _header->status_code;
    switch(_header->response_type)
    {
    case mu_mqp_error:
        qheader.response_type = MqpResponseType::error;
        break;
    case mu_mqp_matched:
        qheader.response_type = MqpResponseType::matched;
        break;
    case mu_mqp_deleted:
        qheader.response_type = MqpResponseType::deleted;
        break;
    default:
        break;
    }
    emit connected(qheader);
}

void MqpClient::onMessageReceived(const MU_MqpMessage * _message)
{
    MqpRawMessage qmessage;
    qmessage.body = _message->body;
    qmessage.subject = _message->subject;
    qmessage.id = _message->id;
    size_t count = muStringListLength(_message->from);
    for(size_t i = 0; i < count; ++i)
        qmessage.from.append(muStringListString(_message->from, i));
    count = muStringListLength(_message->to);
    for(size_t i = 0; i < count; ++i)
        qmessage.to.append(muStringListString(_message->to, i));
    count = muStringListLength(_message->cc);
    for(size_t i = 0; i < count; ++i)
        qmessage.cc.append(muStringListString(_message->cc, i));
    count = muStringListLength(_message->bcc);
    for(size_t i = 0; i < count; ++i)
        qmessage.bcc.append(muStringListString(_message->bcc, i));
    emit messageReceived(qmessage);
}

void MqpClient::onError(unsigned int _error_code)
{
    qDebug() << "MQP Error: " << _error_code;
}

void MqpClient::onFinished()
{
    emit finished();
}
