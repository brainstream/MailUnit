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

#ifndef __MUGUI_MQPCLIENT_SERVERCONFIG_H__
#define __MUGUI_MQPCLIENT_SERVERCONFIG_H__

#include <QString>
#include <QObject>
#include <QVector>

namespace MailUnit {
namespace Gui {

class ServerConfig
{
public:
    ServerConfig(const QString & _name, const QString & _host, quint16 _port) :
        m_name(_name),
        m_host(_host),
        m_port(_port)
    {
    }

    ServerConfig(const ServerConfig &) = default;

    ServerConfig & operator = (const ServerConfig &) = default;

    const QString & name() const
    {
        return m_name;
    }

    void setName(const QString & _name)
    {
        m_name = _name;
    }

    const QString & host() const
    {
        return m_host;
    }

    void setHost(const QString & _host)
    {
        m_host = _host;
    }

    quint16 port() const
    {
        return m_port;
    }

    void setPort(quint16 _port)
    {
        m_port = _port;
    }

private:
    QString m_name;
    QString m_host;
    quint16 m_port;
}; // class ServerConfig


class ServerConfigList : public QObject
{
    Q_OBJECT

public:
    ServerConfigList(QObject * _parent = nullptr) :
        QObject(_parent)
    {
    }

    ~ServerConfigList() override;

    int size() const
    {
        return m_servers.size();
    }

    const ServerConfig & at(int _index) const
    {
        return *m_servers.at(_index);
    }

    const ServerConfig & operator [](int _index) const
    {
        return at(_index);
    }

    bool update(int _index, const ServerConfig & _server);

public slots:
    void add(const ServerConfig & _server);
    bool remove(int _index);

signals:
    void added(const ServerConfig & _server);
    void removed(const ServerConfig & _server);
    void updated(const ServerConfig & _server);

private:
    QVector<ServerConfig *> m_servers;
}; // class ServerConfigList

} // namespace Gui
} // namespace MailUnit


#endif // __MUGUI_MQPCLIENT_SERVERCONFIG_H__
