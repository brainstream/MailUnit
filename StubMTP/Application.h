#ifndef __STUBMTP_APPLICATION_H__
#define __STUBMTP_APPLICATION_H__

#include <StubMTP/Aux.h>
#include <StubMTP/Config.h>
#include <StubMTP/Logger.h>
#include <StubMTP/Exception.h>

namespace StubMTP {

STUBMTP_EXCEPTION(ApplicationException)

class Application final
{
    STUBMTP_DISABLE_COPY(Application)

public:
    Application(const Config & _config, Logger & _logger) :
        mr_config(_config),
        mr_logger(_logger)
    {
    }

    const Config & config() const
    {
        return mr_config;
    }

    Logger & log()
    {
        return mr_logger;
    }

private:
    const Config & mr_config;
    Logger & mr_logger;
}; // class Application

Application & app();

} // namespace StubMTP

#endif // __STUBMTP_APPLICATION_H__
