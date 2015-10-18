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

#include <LibMailUnit/Api/Message/Mailbox.h>

using namespace LibMailUnit::Mail;

const MU_MailboxGroup * muMailboxGroupParse(const char * _raw_address_group)
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

size_t MU_CALL muMailboxCount(const MU_MailboxGroup * _mailbox_group)
{
    if(nullptr == _mailbox_group)
        return 0;
    return _mailbox_group->pointer()->mailboxCount();
}

const MU_Mailbox * muMailbox(const MU_MailboxGroup * _mailbox_group, size_t _index)
{
    if(nullptr == _mailbox_group)
        return nullptr;
    const MailboxGroup & group = *_mailbox_group->pointer();
    const Mailbox * mailbox = &group[_index];
    return _index >= group.mailboxCount() ? nullptr : new MU_Mailbox(mailbox, false);
}

const char * MU_CALL muMailboxGroupName(const MU_MailboxGroup * _mailbox_group)
{
    if(nullptr == _mailbox_group)
        return nullptr;
    const MailboxGroup * group = _mailbox_group->pointer();
    return group->name().empty() ? nullptr : group->name().c_str();
}

const char * MU_CALL muMailboxName(const MU_Mailbox * _mailbox)
{
    if(nullptr == _mailbox)
        return nullptr;
    const Mailbox * mailbox = _mailbox->pointer();
    return mailbox->name().empty() ? nullptr : mailbox->name().c_str();
}

const char * MU_CALL muMailboxAddress(const MU_Mailbox * _mailbox)
{
    if(nullptr == _mailbox)
        return nullptr;
    const Mailbox * mailbox = _mailbox->pointer();
    return mailbox->address().empty() ? nullptr : mailbox->address().c_str();
}
