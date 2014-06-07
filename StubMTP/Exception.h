#ifndef __STUBMTP_EXCEPTION_H__
#define __STUBMTP_EXCEPTION_H__

#include <stdexcept>

namespace StubMTP {

class Excpetion : public std::runtime_error
{
public:
    Excpetion(const std::string & _message) :
        runtime_error(_message)
    {
    }
}; // class Excpetion


#define STUBMTP_EXCEPTION(name)              \
    class name : public StubMTP::Excpetion   \
    {                                        \
    public:                                  \
        name(const std::string & _message) : \
            Excpetion(_message)              \
    };

} // namespace StubMTP


#endif // __STUBMTP_EXCEPTION_H__
