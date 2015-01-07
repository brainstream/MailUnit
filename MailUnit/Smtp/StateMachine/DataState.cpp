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

boost::optional<ResponseCode> DataState::processInput(const char * _data, Protocol & _protocol)
{
    if(m_end_of_data)
    {
        return ResponseCode::ok;
    }
    // FIXME: a data must not be saved in the memory!
    m_data += _data;
    size_t end_pos = m_data.find(SMTP_DATA_END);
    if(std::string::npos == end_pos)
    {
        return nullptr;
    }
    m_end_of_data = true;
    m_data.resize(end_pos + sizeof(SMTP_DATA_END));
    _protocol.email().data() << m_data;
    m_data.clear();
    _protocol.endMessageData();
    _protocol.storeEmail();
    return ResponseCode::ok;
}
