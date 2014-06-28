/***********************************************************************************************
 *                                                                                             *
 * This file is part of MailUnit.                                                               *
 *                                                                                             *
 * MailUnit is free software: you can redistribute it and/or modify it under the terms of       *
 * the GNU General Public License as published by the Free Software Foundation,                *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * MailUnit is distributed in the hope that it will be useful,  but WITHOUT ANY WARRANTY;       *
 * without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  *
 * See the GNU General Public License for more details.                                        *
 *                                                                                             *
 * You should have received a copy of the GNU General Public License along with MailUnit.       *
 * If not, see <http://www.gnu.org/licenses/>.                                                 *
 *                                                                                             *
 ***********************************************************************************************/

#ifndef __STUBMPT_EMAIL_ADDRESS_H__
#define __STUBMPT_EMAIL_ADDRESS_H__

#include <memory>
#include <string>
#include <vector>
#include <ostream>
#include <MailUnit/Server/Aux.h>

/*
 * Most important RFC parts:
 *
 * RFC 5322 - Internet Message Format. Section 3.4 - Address Specification
 *     http://tools.ietf.org/html/rfc5322#section-3.4
 */

namespace MailUnit {
namespace Email {

class Address;
class AddressGroup;
typedef std::shared_ptr<Address>      AddressPtr;
typedef std::shared_ptr<AddressGroup> AddressGroupPtr;

class Address
{
private:
    Address() { }

public:
    MU_DEFAULT_COPY(Address)

    const std::string & name() const
    {
        return m_name;
    }

    const std::string & mailbox() const
    {
        return m_mailbox;
    }

public:
    static AddressPtr parse(const std::string & _input);
    bool compareMailbox(const std::string & _mailbox) const;

private:
    std::string m_name;
    std::string m_mailbox;
}; // class Address


class AddressGroup
{
public:
    MU_DEFAULT_COPY(AddressGroup)

    AddressGroup()
    {
    }

    explicit AddressGroup(const std::string & _name) :
        m_name(_name)
    {
    }

    const std::string & name() const
    {
        return m_name;
    }

    const std::vector<AddressPtr> addresses() const
    {
        return m_addresses;
    }

    bool append(AddressPtr _address)
    {
        if(nullptr == _address || _address->mailbox().empty())
            return false;
        m_addresses.push_back(_address);
        return true;
    }

    bool empty() const
    {
        return m_addresses.empty();
    }

public:
    static AddressGroupPtr parse(const std::string & _input);
    bool containsMailbox(const std::string & _mailbox) const;

private:
    std::string m_name;
    std::vector<AddressPtr> m_addresses;
}; // class AddressGroup

inline AddressGroupPtr makeEmptyAddressGroupPtr()
{
    return std::make_shared<AddressGroup>();
}

inline AddressGroupPtr makeEmptyAddressGroupPtr(const std::string & _name)
{
    return std::make_shared<AddressGroup>(_name);
}

} // namespace Email
} // namespace MailUnit


std::ostream & operator << (std::ostream & _stream, const MailUnit::Email::Address & _address);

inline std::ostream & operator << (std::ostream & _stream, const MailUnit::Email::AddressPtr & _address)
{
    if(_address)
        _stream << *_address;
    return _stream;
}

std::ostream & operator << (std::ostream & _stream, const MailUnit::Email::AddressGroup & _group);

inline std::ostream & operator << (std::ostream & _stream, const MailUnit::Email::AddressGroupPtr & _group)
{
    if(_group)
        _stream << *_group;
    return _stream;
}

#endif // __STUBMPT_EMAIL_ADDRESS_H__
