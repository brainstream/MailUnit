/***********************************************************************************************
 *                                                                                             *
 * This file is part of MailUnit.                                                              *
 *                                                                                             *
 * MailUnit is free software: you can redistribute it and/or modify it under the terms of      *
 * the GNU General Public License as published by the Free Software Foundation,                *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * MailUnit is distributed in the hope that it will be useful,  but WITHOUT ANY WARRANTY;      *
 * without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  *
 * See the GNU General Public License for more details.                                        *
 *                                                                                             *
 * You should have received a copy of the GNU General Public License along with MailUnit.      *
 * If not, see <http://www.gnu.org/licenses/>.                                                 *
 *                                                                                             *
 ***********************************************************************************************/

#ifndef __MU_LOGGER_H__
#define __MU_LOGGER_H__

#include <sstream>
#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>
#include <MailUnit/Exception.h>


namespace MailUnit {

enum class LogLevel
{
    info,
    warning,
    error
}; // enum class LogLevel


MU_EXCEPTION(LoggerException)

class Logger final : private boost::noncopyable
{
public:
    static const boost::uintmax_t min_filesize = 1024 * 1024;
    static const boost::uintmax_t defult_max_filesize = 10 * 1024 * 1024;

    struct Options
    {
        Options() :
            min_level(LogLevel::info),
            max_filesize(Logger::defult_max_filesize),
            stdlog(false)
        {
        }

        Options(const Options &) = default;

        Options & operator = (const Options &) = default;

        boost::filesystem::path filepath;
        LogLevel min_level;
        boost::uintmax_t max_filesize;
        bool stdlog;
    }; // struct Options

public:
    explicit Logger(const Options & _options);
    void write(LogLevel _level, const std::string & _message);

private:
    void prepareFile();
    void incrementFileVersion(const boost::filesystem::path & _path);

private:
    Options m_options;
}; // class Logger

extern Logger * const logger;

class LogWriter : private boost::noncopyable
{
public:
    explicit LogWriter(LogLevel _level) :
        m_level(_level)
    {
    }

    ~LogWriter()
    {
        if(nullptr != logger)
            logger->write(m_level, m_stream.str());
    }

    std::ostream & stream()
    {
        return m_stream;
    }

private:
    LogLevel m_level;
    std::stringstream m_stream;
}; // class LogWriter

} // namespace MailUnit

template<typename Type>
inline MailUnit::LogWriter && operator << (MailUnit::LogWriter && _writer, const Type & _value)
{
    _writer.stream() << _value;
    return std::move(_writer);
}

std::ostream & operator << (std::ostream & _stream, MailUnit::LogLevel _level);

#define LOG_INFO  ::MailUnit::LogWriter(::MailUnit::LogLevel::info)
#define LOG_WARN  ::MailUnit::LogWriter(::MailUnit::LogLevel::warning)
#define LOG_ERROR ::MailUnit::LogWriter(::MailUnit::LogLevel::error)

#endif // __MU_LOGGER_H__
