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

#include <boost/algorithm/string.hpp>
#include <MailUnit/Smtp/StateMachine/SingleLineCmdState.h>
#include <MailUnit/Smtp/ProtocolDef.h>

using namespace MailUnit::Smtp;

bool SingleLineCmdState::internalProcessInput(const std::string & _input)
{
    if(m_completed) return true;
    size_t end_pos = _input.find(SMTP_NEWLINE);
    if(std::string::npos == end_pos)
    {
        m_data += _input;
    }
    else
    {
        m_data += _input.substr(0, end_pos);
        m_completed = true;
    }
    return m_completed;
}

std::string SingleLineCmdState::args() const
{
    std::string args = m_data.substr(m_args_pos);
    boost::trim(args);
    return args;
}
