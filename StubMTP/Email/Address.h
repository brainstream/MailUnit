#ifndef __STUBMPT_EMAIL_ADDRESS_H__
#define __STUBMPT_EMAIL_ADDRESS_H__

#include <string>
#include <vector>
#include <ostream>

namespace StubMTP {
namespace Email {


struct Address
{
    std::string name;
    std::string mailbox;

    bool empty() const
    {
        return mailbox.empty();
    }

    void clear()
    {
        name.clear();
        mailbox.clear();
    }
}; // struct Address

struct AddressGroup
{
    std::string name;
    std::vector<Address> addresses;

    bool empty() const
    {
        return addresses.empty();
    }

    void clear()
    {
        name.clear();
        addresses.clear();
    }
}; // struct AddressGroup


bool parseAddressGroup(const std::string & _input, AddressGroup & _output);
bool parseAddress(const std::string & _input, Address & _output);

} // namespace Email
} // namespace StubMTP


std::ostream & operator << (std::ostream & _stream, const StubMTP::Email::Address & _address);
std::ostream & operator << (std::ostream & _stream, const StubMTP::Email::AddressGroup & _group);


#endif // __STUBMPT_EMAIL_ADDRESS_H__
