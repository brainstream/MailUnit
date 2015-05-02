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

#ifndef _MUGUI_MQPCLIENT_H__
#define _MUGUI_MQPCLIENT_H__

#include <QtNetwork/QTcpSocket>
#include <MailUnitUI/ServerConfig.h>
#include <MailUnitUI/Message.h>

namespace MailUnit {
namespace Gui {

class MqpClientNotifier : public QObject
{
    Q_OBJECT

public:
    explicit MqpClientNotifier(QObject * _parent = nullptr) :
        QObject(_parent)
    {
    }

signals:
    void headerReceived(quint32 _status_code, quint32 _afected_count);
    void messageReceived(const Message & _message);
    void finished();
}; // class MqpClientNotifier

class MqpClient : public MqpClientNotifier
{
    Q_OBJECT
    friend void sendMqpRequestAsync(const ServerConfig &, const QString &, MqpClientNotifier *);

private:
    explicit MqpClient(const ServerConfig & _server, const QString & _request);
    quint32 readHeader();
    void readMessage();

public:
    void run();

private slots:
    void onSocketConnected();
    void onSocketReadyRead();
    void onSocketError(QAbstractSocket::SocketError _error);

private:
    QTcpSocket * mp_socket;
    ServerConfig m_server;
    QString m_request;
}; // class MqpClient

void sendMqpRequestAsync(const ServerConfig & _server, const QString & _request, MqpClientNotifier * _notifier = nullptr);

} // namespace Gui
} // namespace MailUnit

#endif // _MUGUI_MQPCLIENT_H__
