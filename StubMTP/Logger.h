#ifndef __STUBMTP_LOGGER_H__
#define __STUBMTP_LOGGER_H__

#include <functional>
#include <ostream>
#include <boost/filesystem.hpp>
#include <StubMTP/Aux.h>
#include <StubMTP/Exception.h>


namespace StubMTP {

enum class LogLevel
{
    Info,
    Warning,
    Error
}; // enum class LogLevel


STUBMTP_EXCEPTION(LoggerException)

class Logger final
{
    STUBMTP_DISABLE_COPY(Logger)

private:
    Logger(const boost::filesystem::path & _filepath, LogLevel _min_level,
        boost::uintmax_t _max_filesize = s_defult_max_filesize);

public:
    static void initSingleton(const boost::filesystem::path & _filepath, LogLevel _min_level,
        boost::uintmax_t _max_filesize = s_defult_max_filesize);
    static Logger & instance();

    void info(const std::string & _message);
    void warning(const std::string & _message);
    void warning(const std::exception & _exception);
    void warning(const std::string & _message, const std::exception & _exception);
    void error(const std::string & _message);
    void error(const std::exception & _exception);
    void error(const std::string & _message, const std::exception & _exception);

private:
    void write(LogLevel _level, std::function<void(std::fstream &)> _callback);
    void prepareFile();
    void incrementFileVersion(const boost::filesystem::path & _path);

public:
    static const boost::uintmax_t s_min_filesize = 1024 * 1024;
    static const boost::uintmax_t s_defult_max_filesize = 10 * 1024 * 1024;

private:
    LogLevel m_min_level;
    boost::filesystem::path m_filepath;
    boost::uintmax_t m_max_file_size;
}; // class Logger


inline Logger & log()
{
    return Logger::instance();
}


} // namespace StubMTP


std::ostream & operator << (std::ostream & _stream, StubMTP::LogLevel _level);


#endif // __STUBMTP_LOGGER_H__
