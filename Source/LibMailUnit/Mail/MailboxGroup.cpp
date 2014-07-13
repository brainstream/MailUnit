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
#include <LibMailUnit/Mail/MailboxGroup.h>

using namespace LibMailUnit::Mail;

MailboxGroup::MailboxGroup(const std::string & _input)
{
    // TODO: process the case when the colon symbol is part of the quoted string or domain
    size_t colon_pos = _input.find(':');
    if(std::string::npos != colon_pos)
        m_name = boost::trim_copy(_input.substr(0, colon_pos));
    std::vector<std::string> raw_addresses;
    boost::algorithm::split(raw_addresses,
        std::string::npos == colon_pos ? _input : _input.substr(colon_pos + 1),
        [](char symbol) { return ',' == symbol; });
    for(const std::string & raw_address : raw_addresses)
    {
        std::shared_ptr<Mailbox> mailbox = Mailbox::parse(raw_address);
        if(nullptr != mailbox)
            m_mailboxes.push_back(mailbox);
    }
}
