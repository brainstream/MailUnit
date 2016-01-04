/***********************************************************************************************
 *                                                                                             *
 * This file is part of the MailUnit Library.                                                  *
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

#include <LibMailUnit/Api/Impl/StringList.h>
#include <LibMailUnit/Api/Impl/Mqp/Client.h>

using namespace LibMailUnit::Mqp;

namespace {

class CallbackProxy : public CommandExecutionObserver
{
public:
    CallbackProxy(MU_MqpProc _proc, void * _user_data);
    CallbackProxy(const CallbackProxy &) = default;
    CallbackProxy & operator =(const CallbackProxy &) = default;
    void onError(const Command & _command, const boost::system::error_code & _error) override;
    void onResponseHeaderReceived(const Command & _command, const ResponseHeader & _header) override;
    void onMessageReceived(const Command & _command, const Message & _message) override;
    void onFinished(const Command & _command) override;

private:
    MU_MqpProc m_proc;
    void * mp_user_data;
}; // class CallbackProxy

CallbackProxy::CallbackProxy(MU_MqpProc _proc, void * _user_data) :
    m_proc(_proc),
    mp_user_data(_user_data)
{
}

void CallbackProxy::onError(const Command & _command, const boost::system::error_code & _error)
{
    MU_UNUSED(_command);
    if(m_proc)
    {
        int error_code = _error.value();
        m_proc(mu_mqp_evt_error, &error_code, mp_user_data);
    }
}

void CallbackProxy::onResponseHeaderReceived(const Command & _command, const ResponseHeader & _header)
{
    MU_UNUSED(_command);
    if(m_proc)
    {
        MU_MqpResponseHeader api_header = { };
        api_header.affected_count = _header.affected_count;
        api_header.status_code = _header.status_code;
        switch(_header.response_type)
        {
        case ResponseType::matched:
            api_header.response_type = mu_mqp_matched;
            break;
        case ResponseType::deleted:
            api_header.response_type = mu_mqp_deleted;
            break;
        case ResponseType::error:
            api_header.response_type = mu_mqp_error;
            break;
        default:
            break;
        }
        m_proc(mu_mqp_evt_response_header, &api_header, mp_user_data);
    }
}

void CallbackProxy::onMessageReceived(const Command & _command, const Message & _message)
{
    MU_UNUSED(_command);
    if(m_proc)
    {
        MU_MqpMessage api_msg_header = { };
        api_msg_header.id = _message.id;
        api_msg_header.subject = _message.subject.c_str();
        MU_StringList from(&_message.from, false);
        MU_StringList to(&_message.to, false);
        MU_StringList cc(&_message.cc, false);
        MU_StringList bcc(&_message.bcc, false);
        api_msg_header.from = &from;
        api_msg_header.to = &to;
        api_msg_header.cc = &cc;
        api_msg_header.bcc = &bcc;
        api_msg_header.body = _message.body;
        m_proc(mu_mqp_evt_message, &api_msg_header, mp_user_data);
    }
}

void CallbackProxy::onFinished(const Command & _command)
{
    MU_UNUSED(_command);
    if(m_proc)
    {
        m_proc(mu_mqp_evt_finish, nullptr, mp_user_data);
    }
}

class CommandProxy : public Command
{
public:
    CommandProxy(const std::string & _query, unsigned short _options, MU_MqpProc _proc, void * _user_data) :
        Command(_query, _options),
        mp_callback_proxy(new CallbackProxy(_proc, _user_data))
    {
        this->subscribe(*mp_callback_proxy);
    }

    ~CommandProxy() override
    {
        delete mp_callback_proxy;
    }

private:
    CallbackProxy * mp_callback_proxy;
}; // class CommandProxy

} // namespace

MU_MqpClient * MU_CALL muMqpCreateClient(const char * _host, unsigned short _port)
{
    return new MU_MqpClient(new Client(_host, _port), true);
}

MU_MqpCommand * MU_CALL muMqpSendCommand(MU_MqpClient * _client, const char * _cmd, MU_MqpProc _proc, void * _user_data)
{
    if(nullptr == _client)
        return nullptr;
    Command * command = new CommandProxy(_cmd, 0, _proc, _user_data);
    _client->pointer()->executeCommand(*command);
    return new MU_MqpCommand(command, true);
}
