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

void MqpClient::query(const QString & _query)
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
    m_query = _query.trimmed();
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
    delete mp_socket;
    mp_socket = nullptr;
}

void MqpClient::onSocketReadyRead()
{
    QString data = QString::fromUtf8(mp_socket->readAll());
    emit messageReceived(data);
}

void MqpClient::onSocketError(QAbstractSocket::SocketError _error)
{

}
