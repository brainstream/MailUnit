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
    mp_messages(nullptr),
    m_server(_server)
{
    mp_messages = new MqpMessageRepository(this);
}

void MqpClient::executeRequest(const QString & _request)
{
    QString request = _request.trimmed();
    if(!request.endsWith(';'))
        request += ';';
    QTcpSocket * socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, [socket, request]() {
        socket->write(request.toUtf8());
    });
    QMetaObject::Connection ready_con = connect(socket, &QTcpSocket::readyRead, [this, socket, &ready_con]() {
        disconnect(ready_con);
        quint32 message_count = readHeader(*socket);
        for(quint32 i = 0; i < message_count; ++i)
            readMessage(*socket);
        socket->write("quit;");
    });
    connect(socket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
        [this](QAbstractSocket::SocketError) {
        // TODO: handle
    });
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    connect(socket, SIGNAL(disconnected()), SIGNAL(finished()));
    socket->connectToHost(m_server.host(), m_server.port());
}

quint32 MqpClient::readHeader(QTcpSocket & _socket)
{
    static const QString hdr_status  = "STATUS: ";
    static const QString hdr_matched = "MATCHED: ";
    static const QString hdr_deleted = "DELETED: ";
    MqpResponseHeader header = { };
    header.response_type = MqpResponseType::error;
    bool has_messages = false;
    for(;;)
    {
        if(!_socket.canReadLine()) {
            _socket.waitForReadyRead();
            continue;
        }
        QString line(_socket.readLine());
        line = line.trimmed();
        if(line.isEmpty()) break;
        if(line.startsWith(hdr_status))
        {
            header.status_code = line.mid(hdr_status.length()).toUInt();
        }
        else if(line.startsWith(hdr_matched))
        {
            has_messages = true;
            header.affected_count = line.mid(hdr_matched.length()).toUInt();
            header.response_type = MqpResponseType::matched;
        }
        else if(line.startsWith(hdr_deleted))
        {
            header.affected_count = line.mid(hdr_matched.length()).toUInt();
            header.response_type = MqpResponseType::deleted;
        }
    }
    emit connected(header);
    return has_messages ? header.affected_count : 0;
}

void MqpClient::readMessage(QTcpSocket & _socket)
{
    static const QString hdr_item    = "ITEM: ";
    static const QString hdr_size    = "SIZE: ";
    static const QString hdr_id      = "ID: ";
    static const QString hdr_subject = "SUBJECT: ";
    static const QString hdr_from    = "FROM: ";
    static const QString hdr_to      = "TO: ";
    static const QString hdr_cc      = "CC: ";
    static const QString hdr_bcc     = "BCC: ";
    Message message =  { };
    size_t size = 0;
    for(;;)
    {
        if(!_socket.canReadLine()) {
            _socket.waitForReadyRead();
            continue;
        }
        QString line(_socket.readLine());
        line = line.trimmed();
        if(line.isEmpty()) break;
        if(line.startsWith(hdr_item))
        {
            // TODO: parse
        }
        else if(line.startsWith(hdr_size))
        {
            size = line.mid(hdr_size.length()).toULong();
        }
        else if(line.startsWith(hdr_id))
        {
            message.id = line.mid(hdr_id.length()).toUInt();
        }
        else if(line.startsWith(hdr_from))
        {
            message.from.append(line.mid(hdr_from.length()));
        }
        else if(line.startsWith(hdr_to))
        {
            message.to.append(line.mid(hdr_to.length()));
        }
        else if(line.startsWith(hdr_cc))
        {
            message.cc.append(line.mid(hdr_cc.length()));
        }
        else if(line.startsWith(hdr_bcc))
        {
            message.bcc.append(line.mid(hdr_bcc.length()));
        }
        else if(line.startsWith(hdr_subject))
        {
            message.subject = line.mid(hdr_subject.length());
        }
    }
    for(size_t size_left = size; size_left > 0;)
    {
        if(_socket.atEnd()) {
            _socket.waitForReadyRead();
        }
        QByteArray data = _socket.read(size_left);
        message.body.append(data);
        size_left -= data.length();
    }
    emit messageReceived(message);
}

