#ifndef __STUBMTP_APPLICATION_H__
#define __STUBMTP_APPLICATION_H__

#include <boost/filesystem.hpp>
#include <StubMTP/Aux.h>
#include <StubMTP/Config.h>
#include <StubMTP/Logger.h>
#include <StubMTP/Exception.h>

namespace StubMTP {

STUBMTP_EXCEPTION(ApplicationException)

class Application
{
    STUBMTP_DISABLE_COPY(Application)

public:
    Application(int _argc, const char **_argv);
    ~Application();

    const Config & config() const
    {
        return *mp_config;
    }

    Logger & log()
    {
        return *mp_logger;
    }

    const boost::filesystem::path & startDir() const
    {
        return mr_start_dir;
    }

private:
    Config * mp_config;
    Logger * mp_logger;
    const boost::filesystem::path & mr_start_dir;
}; // class Application

Application & app();

} // namespace StubMTP

#endif // __STUBMTP_APPLICATION_H__
