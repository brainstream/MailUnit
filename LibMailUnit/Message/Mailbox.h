/***********************************************************************************************
 *                                                                                             *
 * This file is part of the MailUnit Library.                                                  *
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

#ifndef __LIBMU_MESSAGE_MAILBOX_H__
#define __LIBMU_MESSAGE_MAILBOX_H__

#include <memory>
#include <string>
#include <vector>

namespace LibMailUnit {
namespace Message {

class Mailbox final
{
public:
    explicit Mailbox(const std::string & _address, const std::string & _name = std::string()) :
        m_name(_name),
        m_address(_address)
    {
    }

    Mailbox(const Mailbox &) = default;

    Mailbox & operator = (const Mailbox &) = default;

    const std::string & name() const
    {
        return m_name;
    }

    const std::string & address() const
    {
        return m_address;
    }

    static std::unique_ptr<Mailbox> parse(const std::string & _input);

private:
    std::string m_name;
    std::string m_address;
}; // class Mailbox


class MailboxGroup final
{
public:
    MailboxGroup(const std::string & _input);

    MailboxGroup(const MailboxGroup & _group);

    MailboxGroup(MailboxGroup && _group);

    ~MailboxGroup();

    MailboxGroup & operator = (const MailboxGroup & _group);

    MailboxGroup & operator = (MailboxGroup && _group);

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

    const Mailbox & operator [](size_t _index) const
    {
        return *m_mailboxes[_index];
    }

private:
    void release();

private:
    std::string m_name;
    std::vector<Mailbox *> m_mailboxes;
}; // class MailboxGroup

} // namespace Message
} // namespace LibMailUnit

#endif // __LIBMU_MESSAGE_MAILBOX_H__
