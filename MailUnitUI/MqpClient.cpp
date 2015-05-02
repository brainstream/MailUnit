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

#include <MailUnitUI/MqpClient.h>

using namespace MailUnit::Gui;

MqpClient::MqpClient(const ServerConfig & _config, QObject * _parent /*= nullptr*/) :
    QObject(_parent),
    m_hostname(_config.host()),
    m_port(_config.port()),
    mp_socket(nullptr)
{
}

void MqpClient::sendRequest(const QString & _request)
{
    if(busy())
    {
        throw std::runtime_error("MQP Client is busy"); // FIXME: custom exception
    }
    mp_socket = new QTcpSocket(this);
    connect(mp_socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    connect(mp_socket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
    connect(mp_socket, SIGNAL(error(QAbstractSocket::SocketError)),
        this, SLOT(onSocketError(QAbstractSocket::SocketError)));
    connect(mp_socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
    m_query = _request.trimmed();
    if(!m_query.endsWith(';'))
    {
        m_query += ';';
    }
    mp_socket->connectToHost(m_hostname, m_port);
}

void MqpClient::onSocketConnected()
{
    mp_socket->write(m_query.toUtf8());
}


void MqpClient::onSocketDisconnected()
{
    mp_socket->deleteLater();
    mp_socket = nullptr;
}

void MqpClient::onSocketReadyRead()
{
    disconnect(mp_socket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
    quint32 message_count = readHeader();
    for(quint32 i = 0; i < message_count; ++i)
    {
        readMessage();
    }
    mp_socket->write("quit;");
}

void MqpClient::onSocketError(QAbstractSocket::SocketError _error)
{

}

quint32 MqpClient::readHeader()
{
    static const QString hdr_status  = "STATUS: ";
    static const QString hdr_matched = "MATCHED: ";
    static const QString hdr_deleted = "DELETED: ";
    quint32 status_code = 0;
    quint32 affected_count = 0;
    bool has_messages = false;
    for(;;)
    {
        if(!mp_socket->canReadLine()) {
            mp_socket->waitForReadyRead();
            continue;
        }
        QString line(mp_socket->readLine());
        line = line.trimmed();
        if(line.isEmpty()) break;
        if(line.startsWith(hdr_status))
        {
            status_code = line.mid(hdr_status.length()).toUInt();
        }
        else if(line.startsWith(hdr_matched))
        {
            has_messages = true;
            affected_count = line.mid(hdr_matched.length()).toUInt();
        }
        else if(line.startsWith(hdr_deleted))
        {
            affected_count = line.mid(hdr_matched.length()).toUInt();
        }
    }
    emit headerReceived(status_code, affected_count);
    return has_messages ? affected_count : 0;
}

void MqpClient::readMessage()
{
    static const QString hdr_item    = "ITEM: ";
    static const QString hdr_size    = "SIZE: ";
    static const QString hdr_id      = "ID: ";
    static const QString hdr_subject = "SUBJECT: ";
    static const QString hdr_from    = "FROM: ";
    static const QString hdr_to      = "TO: ";
    static const QString hdr_cc      = "CC: ";
    static const QString hdr_bcc     = "BCC: ";
    Message message;
    size_t size = 0;
    for(;;)
    {
        if(!mp_socket->canReadLine()) {
            mp_socket->waitForReadyRead();
            continue;
        }
        QString line(mp_socket->readLine());
        line = line.trimmed();
        if(line.isEmpty()) break;
        if(line.startsWith(hdr_item))
        {

        }
        else if(line.startsWith(hdr_size))
        {
            size = line.mid(hdr_size.length()).toULong();
        }
        else if(line.startsWith(hdr_id))
        {

        }
        else if(line.startsWith(hdr_from))
        {

        }
        else if(line.startsWith(hdr_cc))
        {

        }
        else if(line.startsWith(hdr_bcc))
        {

        }
        else if(line.startsWith(hdr_subject))
        {
            message.subject = line.mid(hdr_subject.length());
        }
    }
    for(size_t size_left = size; size_left > 0;)
    {
        if(mp_socket->atEnd()) {
            mp_socket->waitForReadyRead();
        }
        QByteArray data = mp_socket->read(size_left);
        message.body.append(data);
        size_left -= data.length();
    }
    emit messageReceived(message);
}
