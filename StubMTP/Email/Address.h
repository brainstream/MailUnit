#ifndef __STUBMPT_EMAIL_ADDRESS_H__
#define __STUBMPT_EMAIL_ADDRESS_H__

#include <memory>
#include <string>
#include <vector>
#include <ostream>
#include <StubMTP/Aux.h>

namespace StubMTP {
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
    STUBMTP_DEFAULT_COPY(Address)

    static AddressPtr parse(const std::string & _input);

    const std::string & name() const
    {
        return m_name;
    }

    const std::string & mailbox() const
    {
        return m_mailbox;
    }

    bool compareMailbox(const std::string & _mailbox) const;

private:
    std::string m_name;
    std::string m_mailbox;
}; // class Address


class AddressGroup
{
public:
    AddressGroup()
    {
    }

    STUBMTP_DEFAULT_COPY(AddressGroup)

    static AddressGroupPtr parse(const std::string & _input);

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

    bool containsMailbox(const std::string & _mailbox) const;

private:
    std::string m_name;
    std::vector<AddressPtr> m_addresses;
}; // class AddressGroup

inline AddressGroupPtr makeEmptyAddressGroupPtr()
{
    return std::make_shared<AddressGroup>();
}

} // namespace Email
} // namespace StubMTP


std::ostream & operator << (std::ostream & _stream, const StubMTP::Email::Address & _address);

inline std::ostream & operator << (std::ostream & _stream, const StubMTP::Email::AddressPtr & _address)
{
    if(_address)
        _stream << *_address;
    return _stream;
}

std::ostream & operator << (std::ostream & _stream, const StubMTP::Email::AddressGroup & _group);

inline std::ostream & operator << (std::ostream & _stream, const StubMTP::Email::AddressGroupPtr & _group)
{
    if(_group)
        _stream << *_group;
    return _stream;
}

#endif // __STUBMPT_EMAIL_ADDRESS_H__
