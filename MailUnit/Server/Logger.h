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

#include <functional>
#include <ostream>
#include <boost/filesystem.hpp>
#include <MailUnit/Server/Aux.h>
#include <MailUnit/Server/Exception.h>


namespace MailUnit {

enum class LogLevel
{
    Info,
    Warning,
    Error
}; // enum class LogLevel


MU_EXCEPTION(LoggerException)

class Logger final
{
    MU_DISABLE_COPY(Logger)

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


} // namespace MailUnit


std::ostream & operator << (std::ostream & _stream, MailUnit::LogLevel _level);


#endif // __MU_LOGGER_H__
