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

#include <boost/algorithm/string.hpp>
#include <LibMailUnit/Mail/Mailbox.h>

using namespace LibMailUnit::Mail;

std::shared_ptr<Mailbox> Mailbox::parse(const std::string & _input)
{
    // TODO: process the case when the angle symbol is part of the quoted string
    size_t open_angle_pos = _input.find('<');
    if(std::string::npos == open_angle_pos)
    {
        return std::make_shared<Mailbox>(boost::algorithm::trim_copy(_input));
    }
    size_t close_angle_pos = _input.find('>');
    if(std::string::npos == close_angle_pos)
    {
        return nullptr;
    }
    size_t mailbox_start_pos = open_angle_pos + 1;
    size_t mailbox_len = close_angle_pos - mailbox_start_pos;
    std::string name = boost::algorithm::trim_copy(_input.substr(0, open_angle_pos));
    std::string address = boost::algorithm::trim_copy(_input.substr(mailbox_start_pos, mailbox_len));
    return std::make_shared<Mailbox>(address, name);
}
