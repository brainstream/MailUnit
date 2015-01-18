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

#include <cstring>
#include <MailUnit/Logger.h>
#include <MailUnit/Smtp/Protocol.h>
#include <MailUnit/Smtp/StateMachine/StateMachine.h>

using namespace MailUnit::Smtp;
using namespace MailUnit::Storage;

bool ProtocolTransport::callNextAction()
{
    if(m_next_actions.empty())
    {
        return false;
    }
    Action action = m_next_actions.back();
    m_next_actions.pop();
    if(nullptr == action)
    {
        return callNextAction();
    }
    action();
    return true;
}

struct Protocol::Data
{
    Data(Repository & _repository, ProtocolTransport & _transport) :
        parse_verbs(true),
        state_completed(false),
        protocol_completed(false),
        repository(_repository),
        transport(_transport),
        email_ptr(_repository.createRawEmail()),
        state_machine_ptr(new StateMachine())
    {
    }

    bool parse_verbs;
    bool state_completed;
    bool protocol_completed;
    Repository & repository;
    ProtocolTransport & transport;
    std::unique_ptr<RawEmail> email_ptr;
    std::unique_ptr<StateMachine> state_machine_ptr;
}; // struct Protocol::Data

Protocol::Protocol(Repository & _repository, ProtocolTransport & _transport) :
    mp_data(new Data(_repository, _transport))
{
}

Protocol::~Protocol()
{
    delete mp_data;
}

void Protocol::start()
{
    mp_data->state_machine_ptr->process_event(InitEvent());
    StateBase * state = mp_data->state_machine_ptr->currentState();
    processResponseCode(state->processInput(nullptr, *this));
}

void Protocol::processInput(const char * _data)
{
    StateBase * current_state = mp_data->state_machine_ptr->currentState();
    if(nullptr == current_state)
    {
        mp_data->state_completed = true;
        mp_data->transport.addNextAction([this]() {
            mp_data->transport.exitRequest();
        });
        mp_data->transport.writeRequest(translateResponseCode(ResponseCode::internalError));
        return;
    }
    try
    {
        if(mp_data->state_completed)
            nextState(_data);
        else
            continueState(_data);
    }
    catch(const StateException & error)
    {
        mp_data->state_completed = true;
        mp_data->state_machine_ptr->process_event(ErrorEvent());
        mp_data->transport.addNextAction([this]() {
            mp_data->transport.readRequest();
        });
        mp_data->transport.writeRequest(translateResponseCode(error.responseCode()));
    }
}

void Protocol::nextState(const char * _data)
{
    if(!mp_data->parse_verbs)
    {
        mp_data->state_machine_ptr->process_event(DataEvent());
    }
    else if(std::strncmp(_data, SMTP_CMD_EHLO, SMTP_CMDLEN) == 0)
    {
        mp_data->state_machine_ptr->process_event(EhloEvent());
    }
    else if(std::strncmp(_data, SMTP_CMD_MAIL, SMTP_CMDLEN) == 0)
    {
        mp_data->state_machine_ptr->process_event(MailFromEvent());
    }
    else if(std::strncmp(_data, SMTP_CMD_RCPT, SMTP_CMDLEN) == 0)
    {
        mp_data->state_machine_ptr->process_event(RcptToEvent());
    }
    else if(std::strncmp(_data, SMTP_CMD_DATA, SMTP_CMDLEN) == 0)
    {
        mp_data->state_machine_ptr->process_event(DataHeaderEvent());
    }
    else if(std::strncmp(_data, SMTP_CMD_QUIT, SMTP_CMDLEN) == 0)
    {
        mp_data->state_machine_ptr->process_event(QuitEvent());
    }
    else
    {
        mp_data->transport.addNextAction([this]() {
            mp_data->transport.readRequest();
        });
        mp_data->transport.writeRequest(translateResponseCode(ResponseCode::unrecognizedCommand));
        return;
    }
    StateBase * state = mp_data->state_machine_ptr->currentState();
    processResponseCode(state->processInput(_data, *this));
}

void Protocol::continueState(const char * _data)
{
    StateBase * state = mp_data->state_machine_ptr->currentState();
    processResponseCode(state->processInput(_data, *this));
}

void Protocol::processResponseCode(const boost::optional<ResponseCode> & _code)
{
    if(_code.is_initialized())
    {
        mp_data->state_completed = true;
        mp_data->transport.addNextAction([this]() {
            if(mp_data->protocol_completed)
                mp_data->transport.exitRequest();
            else
                mp_data->transport.readRequest();
        });
        mp_data->transport.writeRequest(translateResponseCode(_code.get()));
    }
    else
    {
        mp_data->state_completed = false;
        mp_data->transport.readRequest();
    }
}

RawEmail & Protocol::email()
{
    return *mp_data->email_ptr;
}

void Protocol::beginMessageData()
{
    mp_data->parse_verbs = false;
}

void Protocol::endMessageData()
{
    mp_data->parse_verbs = true;
}

void Protocol::storeEmail()
{
    LOG_INFO << "Message received"; // TODO: more details
    mp_data->repository.storeEmail(*mp_data->email_ptr);
    mp_data->email_ptr = mp_data->repository.createRawEmail();
}

void Protocol::terminate()
{
    mp_data->protocol_completed = true;
}
