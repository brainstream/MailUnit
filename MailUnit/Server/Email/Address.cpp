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

#include <boost/algorithm/string.hpp>
#include <MailUnit/Server/Email/Address.h>

using namespace MailUnit::Email;

/*
 * RFC 5322 3.4 - http://tools.ietf.org/html/rfc5322#section-3.4
 *
 * address         =       mailbox / group
 * mailbox         =       name-addr / addr-spec
 * name-addr       =       [display-name] angle-addr
 * angle-addr      =       [CFWS] "<" addr-spec ">" [CFWS] / obs-angle-addr
 * group           =       display-name ":" [mailbox-list / CFWS] ";"
 *                         [CFWS]
 * display-name    =       phrase
 * mailbox-list    =       (mailbox *("," mailbox)) / obs-mbox-list
 * address-list    =       (address *("," address)) / obs-addr-list
 */

AddressPtr Address::parse(const std::string & _input)
{
    // TODO: process the case when the angle symbol is part of the quoted string
    AddressPtr result(new Address);
    size_t open_angle_pos = _input.find('<');
    if(std::string::npos == open_angle_pos)
    {
        result->m_mailbox = boost::algorithm::trim_copy(_input);
        return result;
    }
    size_t close_angle_pos = _input.find('>');
    if(std::string::npos == close_angle_pos)
    {
        return nullptr;
    }
    size_t mailbox_start_pos = open_angle_pos + 1;
    size_t mailbox_len = close_angle_pos - mailbox_start_pos;
    result->m_name = boost::algorithm::trim_copy(_input.substr(0, open_angle_pos));
    result->m_mailbox = boost::algorithm::trim_copy(_input.substr(mailbox_start_pos, mailbox_len));
    return result;
}

bool Address::compareMailbox(const std::string & _mailbox) const
{
    return boost::algorithm::iequals(m_mailbox, _mailbox);
}

AddressGroupPtr AddressGroup::parse(const std::string & _input)
{
    // TODO: process the case when the colon symbol is part of the quoted string or domain
    AddressGroupPtr result(new AddressGroup);
    size_t colon_pos = _input.find(':');
    if(std::string::npos != colon_pos)
        result->m_name = _input.substr(0, colon_pos);
    std::vector<std::string> raw_addresses;
    boost::algorithm::split(raw_addresses,
        std::string::npos == colon_pos ? _input : _input.substr(colon_pos + 1),
        [](char symbol) { return ',' == symbol; });
    for(const std::string & raw_address : raw_addresses)
    {
        AddressPtr address = Address::parse(raw_address);
        if(address)
        {
            result->m_addresses.push_back(address);
        }
    }
    return result->m_addresses.empty() ? nullptr : result;
}

bool AddressGroup::containsMailbox(const std::string & _mailbox) const
{
    for(const AddressPtr & address : m_addresses)
    {
        if(address->compareMailbox(_mailbox))
            return true;
    }
    return false;
}

std::ostream & operator << (std::ostream & _stream, const Address & _address)
{
    bool is_name_empty = _address.name().empty();
    if(!is_name_empty)
        _stream << _address.name() << ' ';
    if(!is_name_empty)
        _stream << '<';
    _stream << _address.mailbox();
    if(!is_name_empty)
        _stream << '>';
    return _stream;
}

std::ostream & operator << (std::ostream & _stream, const AddressGroup & _group)
{
    if(!_group.name().empty())
        _stream << _group.name() << ": ";
    bool comma = false;
    for(const AddressPtr & address : _group.addresses())
    {
        if(comma)
            _stream << ", ";
        else
            comma = true;
        _stream << address;
    }
    return _stream;
}
