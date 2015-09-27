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

#ifndef _MUGUI_MQPCLIENT_MQPCLIENT_H__
#define _MUGUI_MQPCLIENT_MQPCLIENT_H__

#include <QTcpSocket>
#include <MailUnitUI/MqpClient/ServerConfig.h>
#include <MailUnitUI/MqpClient/MqpMessage.h>

namespace MailUnit {
namespace Gui {

enum class MqpResponseType
{
    matched,
    deleted,
    error
}; // enum class MqpResponseType

struct MqpResponseHeader
{
    MqpResponseHeader()
    {
        static bool registered = false;
        if(!registered)
        {
            registered = true;
            qRegisterMetaType<MqpResponseHeader>("MqpResponseHeader");
        }
    }

    MqpResponseType response_type;
    quint32 status_code;
    quint32 affected_count;
}; // struct MqpResponseHeader

class MqpClient : public QObject
{
    Q_OBJECT

public:
    explicit MqpClient(const ServerConfig & _server, QObject * _parent = nullptr);
    void executeRequest(const QString & _request);

signals:
    void connected(const MqpResponseHeader & _header);
    void messageReceived(const MqpRawMessage & _message);
    void finished();

private:
    quint32 readHeader(QTcpSocket &_socket);
    void readMessage(QTcpSocket &_socket);

private:
    ServerConfig m_server;
}; // class MqpClient

} // namespace Gui
} // namespace MailUnit

#endif // _MUGUI_MQPCLIENT_MQPCLIENT_H__
