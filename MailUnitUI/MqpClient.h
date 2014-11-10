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

#include <QtCore/QString>
#include <QtNetwork/QTcpSocket>

namespace MailUnit {
namespace Gui {

class MqpClient : public QObject
{
    Q_OBJECT

public:
    MqpClient(const QString & _hostname, quint16 _port, QObject * _parent = nullptr);
    void query(const QString & _query);

    bool busy() const
    {
        return nullptr != mp_socket;
    }

    const QString & hostname() const
    {
        return m_hostname;
    }

    bool trySetHostName(const QString & _hostname)
    {
        if(busy())
            return false;
        m_hostname = _hostname;
        return true;
    }

    quint16 port() const
    {
        return m_port;
    }

    bool trySetPort(quint16 _port)
    {
        if(busy())
            return false;
        m_port = _port;
        return true;
    }

signals:
    void messageReceived(const QString & _message);

private slots:
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketReadyRead();
    void onSocketError(QAbstractSocket::SocketError _error);

private:
    QString m_hostname;
    quint16 m_port;
    QTcpSocket * mp_socket;
    QString m_query;
}; // class MqpClient

} // namespace Gui
} // namespace MailUnit

#endif // _MUGUI_MQPCLIENT_H__
