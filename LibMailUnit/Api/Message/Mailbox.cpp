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

#include <LibMailUnit/Memory.h>
#include <LibMailUnit/Mail/Mailbox.h>
#include <Include/LibMailUnit/Message/Mailbox.h>

using namespace LibMailUnit::Mail;

MU_MAILBOXGROUP MU_CALL muMailboxGroupParse(const char * _raw_address_group)
{
    if(nullptr == _raw_address_group)
    {
        return MU_INVALID_HANDLE;
    }
    MailboxGroup * group = new MailboxGroup(_raw_address_group);
    if(group->empty())
    {
        delete group;
        return MU_INVALID_HANDLE;
    }
    return new MHandle(group, true);
}

size_t MU_CALL muMailboxCount(MU_MAILBOXGROUP _mailbox_group)
{
    if(nullptr == _mailbox_group)
        return 0;
    return _mailbox_group->pointer<MailboxGroup>()->mailboxCount();
}

MU_MAILBOX MU_CALL muMailbox(MU_MAILBOXGROUP _mailbox_group, size_t _index)
{
    if(nullptr == _mailbox_group)
        return MU_INVALID_HANDLE;
    MailboxGroup & group = *_mailbox_group->pointer<MailboxGroup>();
    return _index >= group.mailboxCount() ? MU_INVALID_HANDLE : new MHandle(&group[_index], false);
}

const char * MU_CALL muMailboxGroupName(MU_MAILBOXGROUP _mailbox_group)
{
    if(nullptr == _mailbox_group)
        return nullptr;
    MailboxGroup * group = _mailbox_group->pointer<MailboxGroup>();
    return group->name().empty() ? nullptr : group->name().c_str();
}

const char * MU_CALL muMailboxName(MU_MAILBOX _mailbox)
{
    if(nullptr == _mailbox)
        return nullptr;
    Mailbox * mailbox = _mailbox->pointer<Mailbox>();
    return mailbox->name().empty() ? nullptr : mailbox->name().c_str();
}

const char * MU_CALL muMailboxAddress(MU_MAILBOX _mailbox)
{
    if(nullptr == _mailbox)
        return nullptr;
    Mailbox * mailbox = _mailbox->pointer<Mailbox>();
    return mailbox->address().empty() ? nullptr : mailbox->address().c_str();
}
