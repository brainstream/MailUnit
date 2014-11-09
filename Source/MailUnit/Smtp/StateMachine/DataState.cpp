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
#include <MailUnit/Smtp/StateMachine/DataState.h>
#include <MailUnit/Smtp/ProtocolDef.h>

#define COMMAND SMTP_CMD_DATA SMTP_NEWLINE

using namespace MailUnit::Smtp;

void DataState::processInput(const std::string & _input, Storage::RawEmail & _email)
{
    if(m_read_state.error || m_read_state.data_accepted)
        return;
    if(m_read_state.header_accepted)
        processData(_input, _email);
    else
        processHeader(_input);
}

void DataState::processHeader(const std::string & _input)
{
    const size_t cmd_len = sizeof(COMMAND) - 1;
    m_data += _input;
    if(m_data.length() < cmd_len)
        return;
    if(m_data.compare(0, cmd_len, COMMAND) == 0)
        m_read_state.header_accepted = 1;
    else
        m_read_state.error = 1;
}

void DataState::processData(const std::string & _input, Storage::RawEmail & _email)
{
    m_read_state.data_accepting = 1;
    // FIXME: a data must not be saved in the memory!
    m_data += _input;
    size_t end_pos = m_data.find(SMTP_DATA_END);
    if(std::string::npos == end_pos)
    {
        return;
    }
    m_data += _input.substr(0, end_pos);
    m_read_state.data_accepted = 1;
    _email.data() << m_data.substr(sizeof(COMMAND) - 1);
}

bool DataState::response(ResponseCode * _response) const
{
    if(m_read_state.error)
    {
        *_response = ResponseCode::InternalError;
        return true;
    }
    if(m_read_state.data_accepted)
    {
        *_response = ResponseCode::Ok;
        return true;
    }
    else if(m_read_state.data_accepting)
    {
        return false;
    }
    else if(m_read_state.header_accepted)
    {
        *_response = ResponseCode::Intermediate;
        return true;
    }
    *_response = ResponseCode::InternalError;
    return true;
}

void DataState::reset()
{
    std::memset(&m_read_state, 0, sizeof(ReadState));
    m_data.clear();
}
