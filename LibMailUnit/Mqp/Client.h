/***********************************************************************************************
 *                                                                                             *
 * This file is part of MailUnit Library.                                                      *
 *                                                                                             *
 * MailUnit Library is free software: you can redistribute it and/or modify it under the terms *
 * of the GNU Lesser General Public License as published by the Free Software Foundation,      *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * MailUnit Library is distributed in the hope that it will be useful, but WITHOUT ANY         *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  *
 * PURPOSE. See the GNU Lesser General Public License for more details.                        *
 *                                                                                             *
 * You should have received a copy of the GNU License General Public License along with        *
 * MailUnit Library. If not, see <http://www.gnu.org/licenses>.                                *
 *                                                                                             *
 ***********************************************************************************************/

#ifndef __LIBMU_MQP_CLIENT_H__
#define __LIBMU_MQP_CLIENT_H__

#include <string>
#include <set>
#include <Include/LibMailUnit/Mqp/Client.h>
#include <LibMailUnit/ApiObject.h>

namespace LibMailUnit {
namespace Mqp {

typedef MU_MqpResponseHeader ResponseHeader;
typedef MU_MqpResponseType ResponseType;
typedef MU_MqpMessageHeader MessageHeader;

class ClientObserver
{
public:
    virtual ~ClientObserver() { }
    virtual void onResponseHeaderReceived(const ResponseHeader & _header, void * _user_data) = 0;
    virtual void onMessageHeaderReceived(const MessageHeader & _header, void * _user_data) = 0;
}; // class ClientObserver

class Client
{
public:
    Client(const std::string & _host, unsigned short _port);
    bool subscribe(ClientObserver * _observer);
    bool unsubscribe(ClientObserver * _observer);

private:
    std::string m_host;
    unsigned short m_port;
    std::set<ClientObserver *> m_observers;
}; // class Client

} // namespace Mqp
} // namespace LibMailUnit

MU_DEFINE_API_TYPE(MU_MqpClient, LibMailUnit::Mqp::Client)

#endif // __LIBMU_MQP_CLIENT_H__
