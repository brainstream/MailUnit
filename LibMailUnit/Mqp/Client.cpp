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

#include <LibMailUnit/Mqp/Client.h>

using namespace LibMailUnit::Mqp;

namespace {

class MqpClientApiObject : public MU_MqpClient, public ClientObserver
{
public:
    MqpClientApiObject(Client * _client, bool _destructible, MU_MqpProc _proc, void * _user_data) :
        MU_MqpClient(_client, _destructible),
        mp_client(_client)
    {
        mp_client->subscribe(this);
    }

    ~MqpClientApiObject() override
    {
        mp_client->unsubscribe(this);
    }

    void onResponseHeaderReceived(const ResponseHeader & _header, void * _user_data) override
    {
        if(m_proc)
        {
            try
            {
                m_proc(mu_mqp_evt_response_header, &_header, _user_data);
            }
            catch(...)
            {
            }
        }
    }

    void onMessageHeaderReceived(const MessageHeader & _header, void * _user_data) override
    {
        if(m_proc)
        {
            try
            {
                m_proc(mu_mqp_evt_message_header, &_header, _user_data);
            }
            catch(...)
            {
            }
        }
    }

private:
    Client * mp_client;
    MU_MqpProc m_proc;
    void * mp_user_data;
}; // class MqpClientApiObject

} // namespace

Client::Client(const std::string & _host, unsigned short _port) :
    m_host(_host),
    m_port(_port)
{
}

bool Client::subscribe(ClientObserver * _observer)
{
    if(nullptr == _observer)
        return false;
    return m_observers.insert(_observer).second;
}

bool Client::unsubscribe(ClientObserver * _observer)
{
    if(nullptr == _observer)
        return false;
    return m_observers.erase(_observer) > 0;
}

MU_MqpClient * MU_CALL muMqpCreateClient(const char * _host, unsigned short _port, MU_MqpProc _proc, void * _user_data)
{
    Client * client = new Client(_host, _port);
    return new MqpClientApiObject(client, true, _proc, _user_data);
}
