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

std::unique_ptr<Mailbox> Mailbox::parse(const std::string & _input)
{
    // TODO: process the case when the angle symbol is part of the quoted string
    size_t open_angle_pos = _input.find('<');
    if(std::string::npos == open_angle_pos)
    {
        return std::make_unique<Mailbox>(boost::algorithm::trim_copy(_input));
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
    return std::make_unique<Mailbox>(address, name);
}

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
        Mailbox * mailbox = Mailbox::parse(raw_address).release();
        if(nullptr != mailbox)
            m_mailboxes.push_back(mailbox);
    }
}

MailboxGroup::MailboxGroup(const MailboxGroup & _group) :
    m_name(_group.name()),
    m_mailboxes(std::vector<Mailbox *>(_group.m_mailboxes.size()))
{
    for(const Mailbox * mailbox : _group.m_mailboxes)
        m_mailboxes.push_back(new Mailbox(*mailbox));
}

MailboxGroup::MailboxGroup(MailboxGroup && _group) :
    m_name(std::move(_group.name())),
    m_mailboxes(std::move(_group.m_mailboxes))
{
}

MailboxGroup::~MailboxGroup()
{
    release();
}

MailboxGroup & MailboxGroup::operator = (const MailboxGroup & _group)
{
    if(this == &_group)
        return *this;
    m_name = _group.m_name;
    release();
    m_mailboxes.resize(_group.m_mailboxes.size());
    for(const Mailbox * mailbox : _group.m_mailboxes)
        m_mailboxes.push_back(new Mailbox(*mailbox));
    return *this;
}

MailboxGroup & MailboxGroup::operator = (MailboxGroup && _group)
{
    if(this == &_group)
        return *this;
    m_name = std::move(_group.name());
    m_mailboxes = std::move(_group.m_mailboxes);
    return *this;
}

void MailboxGroup::release()
{
    for(const Mailbox * mailbox : m_mailboxes)
        delete mailbox;
}

MU_MailboxGroup * MU_CALL muMailboxGroupParse(const char * _raw_address_group)
{
    if(nullptr == _raw_address_group)
    {
        return nullptr;
    }
    const MailboxGroup * group = new MailboxGroup(_raw_address_group);
    if(group->empty())
    {
        delete group;
        return nullptr;
    }
    return new MU_MailboxGroup(group, true);
}

size_t MU_CALL muMailboxCount(MU_MailboxGroup * _mailbox_group)
{
    if(nullptr == _mailbox_group)
        return 0;
    return _mailbox_group->pointer()->mailboxCount();
}

MU_Mailbox * MU_CALL muMailbox(MU_MailboxGroup * _mailbox_group, size_t _index)
{
    if(nullptr == _mailbox_group)
        return nullptr;
    const MailboxGroup & group = *_mailbox_group->pointer();
    const Mailbox * mailbox = &group[_index];
    return _index >= group.mailboxCount() ? nullptr : new MU_Mailbox(mailbox, false);
}

const char * MU_CALL muMailboxGroupName(MU_MailboxGroup * _mailbox_group)
{
    if(nullptr == _mailbox_group)
        return nullptr;
    const MailboxGroup * group = _mailbox_group->pointer();
    return group->name().empty() ? nullptr : group->name().c_str();
}

const char * MU_CALL muMailboxName(MU_Mailbox * _mailbox)
{
    if(nullptr == _mailbox)
        return nullptr;
    const Mailbox * mailbox = _mailbox->pointer();
    return mailbox->name().empty() ? nullptr : mailbox->name().c_str();
}

const char * MU_CALL muMailboxAddress(MU_Mailbox * _mailbox)
{
    if(nullptr == _mailbox)
        return nullptr;
    const Mailbox * mailbox = _mailbox->pointer();
    return mailbox->address().empty() ? nullptr : mailbox->address().c_str();
}
