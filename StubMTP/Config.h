#ifndef __STUBMTP_CONFIG_H__
#define __STUBMTP_CONFIG_H__

#include <ostream>
#include <boost/program_options.hpp>
#include <StubMTP/Aux.h>
#include <StubMTP/Logger.h>

namespace StubMTP {

class Config;

} // namespace StubMTP


std::ostream & operator << (std::ostream & _stream, const StubMTP::Config & _config);


namespace StubMTP {


class Config final
{
    friend std::ostream & ::operator << (std::ostream & _stream, const StubMTP::Config & _config);
    STUBMTP_DISABLE_COPY(Config)

public:
    Config(int argc, const char ** argv);
    ~Config();

    bool showHelp() const
    {
        return m_show_help;
    }

    uint16_t portNumber() const
    {
        return m_port_number;
    }

    bool logToStdout() const
    {
        return m_log_to_stdout;
    }

    boost::uintmax_t logMaxSize() const
    {
        return m_log_max_size;
    }

    std::string logFilename() const
    {
        return m_log_filename;
    }

    uint16_t threadCount() const
    {
        return m_thread_count;
    }

    LogLevel logLevel() const
    {
        return m_log_level;
    }

private:
    boost::program_options::options_description * mp_description;
    bool m_show_help;
    bool m_log_to_stdout;
    boost::uintmax_t m_log_max_size;
    std::string m_log_filename;
    uint16_t m_port_number;
    uint16_t m_thread_count;
    LogLevel m_log_level;
}; // class Config


} // namespace StubMTP



#endif // __STUBMTP_CONFIG_H__
