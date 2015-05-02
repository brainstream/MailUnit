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

class MqpClient : public QObject
{
    Q_OBJECT

public:
    explicit MqpClient(const ServerConfig & _config, QObject * _parent = nullptr);

    void sendRequest(const QString & _request);

    bool busy() const
    {
        return nullptr != mp_socket;
    }

    const QString & hostname() const
    {
        return m_hostname;
    }

    quint16 port() const
    {
        return m_port;
    }

signals:
    void headerReceived(quint32 _status_code, quint32 _afected_count);
    void messageReceived(const Message & _message);
    void finished();

private slots:
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketReadyRead();
    void onSocketError(QAbstractSocket::SocketError _error);

private:
    quint32 readHeader();
    void readMessage();

private:
    QString m_hostname;
    quint16 m_port;
    QTcpSocket * mp_socket;
    QString m_query;
}; // class MqpClient

} // namespace Gui
} // namespace MailUnit

#endif // _MUGUI_MQPCLIENT_H__
