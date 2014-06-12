#include <boost/algorithm/string.hpp>
#include <StubMTP/Email/Address.h>

using namespace StubMTP::Email;

bool StubMTP::Email::parseAddressGroup(const std::string & _input, AddressGroup & _output)
{
    // TODO: process case when the colon symbol is part of quoted string or domain
    _output.clear();
    size_t colon_pos = _input.find(':');
    if(std::string::npos != colon_pos)
        _output.name = _input.substr(0, colon_pos);
    std::vector<std::string> raw_addresses;
    boost::algorithm::split(raw_addresses,
        std::string::npos == colon_pos ? _input : _input.substr(colon_pos + 1),
        [](char symbol) { return ',' == symbol; });
    for(const std::string & raw_address : raw_addresses)
    {
        Address address;
        if(parseAddress(raw_address, address))
            _output.addresses.push_back(address);
    }
    if(_output.empty())
    {
        _output.clear();
        return false;
    }
    return true;
}

bool StubMTP::Email::parseAddress(const std::string & _input, Address & _output)
{
    // TODO: process case when the angle symbol is part of quoted string
    _output.clear();
    size_t open_angle_pos = _input.find('<');
    if(std::string::npos == open_angle_pos)
    {
        _output.mailbox = boost::algorithm::trim_copy(_input);
        return true;
    }
    size_t close_angle_pos = _input.find('>');
    if(std::string::npos == close_angle_pos)
    {
        _output.clear();
        return false;
    }
    size_t mailbox_start_pos = open_angle_pos + 1;
    size_t mailbox_len = close_angle_pos - mailbox_start_pos;
    _output.name = boost::algorithm::trim_copy(_input.substr(0, open_angle_pos));
    _output.mailbox = boost::algorithm::trim_copy(_input.substr(mailbox_start_pos, mailbox_len));
    return true;
}

std::ostream & operator << (std::ostream & _stream, const Address & _address)
{
    if(_address.empty())
        return _stream;
    bool is_name_empty = _address.name.empty();
    if(!is_name_empty)
        _stream << _address.name << ' ';
    if(!is_name_empty)
        _stream << '<';
    _stream << _address.mailbox;
    if(!is_name_empty)
        _stream << '>';
    return _stream;
}

std::ostream & operator << (std::ostream & _stream, const AddressGroup & _group)
{
    if(_group.empty())
        return _stream;
    if(!_group.name.empty())
        _stream << _group.name << ": ";
    bool comma = false;
    for(const Address & address : _group.addresses)
    {
        if(comma)
            _stream << ", ";
        else
            comma = true;
        _stream << address;
    }
    return _stream;
}
