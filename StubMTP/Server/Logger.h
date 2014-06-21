/***********************************************************************************************
 *                                                                                             *
 * This file is part of StubMTP.                                                               *
 *                                                                                             *
 * StubMTP is free software: you can redistribute it and/or modify it under the terms of       *
 * the GNU General Public License as published by the Free Software Foundation,                *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * StubMTP is distributed in the hope that it will be useful,  but WITHOUT ANY WARRANTY;       *
 * without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  *
 * See the GNU General Public License for more details.                                        *
 *                                                                                             *
 * You should have received a copy of the GNU General Public License along with StubMTP.       *
 * If not, see <http://www.gnu.org/licenses/>.                                                 *
 *                                                                                             *
 ***********************************************************************************************/

#ifndef __STUBMTP_LOGGER_H__
#define __STUBMTP_LOGGER_H__

#include <functional>
#include <ostream>
#include <boost/filesystem.hpp>
#include <StubMTP/Server/Aux.h>
#include <StubMTP/Server/Exception.h>


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

public:
    Logger(const boost::filesystem::path & _filepath, LogLevel _min_level,
        boost::uintmax_t _max_filesize = s_defult_max_filesize);
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


} // namespace StubMTP


std::ostream & operator << (std::ostream & _stream, StubMTP::LogLevel _level);


#endif // __STUBMTP_LOGGER_H__
