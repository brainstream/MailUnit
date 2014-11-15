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

#include <MailUnitUI/ServerConfig.h>

using namespace MailUnit::Gui;

ServerConfigList::~ServerConfigList()
{
    for(ServerConfig * server : m_servers)
        delete server;
}

void ServerConfigList::add(const ServerConfig & _server)
{
    ServerConfig * server = new ServerConfig(_server);
    m_servers.append(server);
    emit added(*server);
}

bool ServerConfigList::remove(int _index)
{
    if(_index < 0 || _index >= m_servers.size())
        return false;
    ServerConfig * server = m_servers.at(_index);
    m_servers.remove(_index);
    emit removed(*server);
    delete server;
    return true;
}

bool ServerConfigList::update(int _index, const ServerConfig & _server)
{
    if(_index < 0 || _index >= m_servers.size())
        return false;
    ServerConfig * server = m_servers.at(_index);
    *server = _server;
    emit updated(*server);
    return true;
}
