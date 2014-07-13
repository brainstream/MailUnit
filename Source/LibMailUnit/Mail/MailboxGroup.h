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

#ifndef __LIBMU_MAIL_MAILBOXGROUP_H__
#define __LIBMU_MAIL_MAILBOXGROUP_H__

#include <vector>
#include <LibMailUnit/Mail/Mailbox.h>

namespace LibMailUnit {
namespace Mail {

class MailboxGroup final
{
public:
    MailboxGroup(const std::string & _input);

    const std::string & name() const
    {
        return m_name;
    }

    size_t mailboxCount() const
    {
        return m_mailboxes.size();
    }

    bool empty() const
    {
        return m_mailboxes.empty();
    }

    Mailbox & operator [](size_t _index)
    {
        return *m_mailboxes[_index].get();
    }

private:
    std::string m_name;
    std::vector<std::shared_ptr<Mailbox>> m_mailboxes;
}; // class MailboxGroup

} // namespace Mail
} // namespace LibMailUnit

#endif // __LIBMU_MAIL_MAILBOXGROUP_H__
