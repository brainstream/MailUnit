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

#include <mutex>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <boost/date_time.hpp>
#include <MailUnit/Logger.h>

using namespace MailUnit;

namespace {

std::mutex log_mutex;

} // namespace


std::ostream & operator << (std::ostream & _stream, LogLevel _level)
{
    switch(_level)
    {
    case LogLevel::info:
        _stream << "INFO";
        break;
    case LogLevel::warning:
        _stream << "WARNING";
        break;
    case LogLevel::error:
        _stream << "ERROR";
        break;
    }
    return _stream;
}


Logger::Logger(const boost::filesystem::path & _filepath, LogLevel _min_level,
        boost::uintmax_t _max_filesize /*= s_defult_max_filesize*/) :
    m_min_level(_min_level),
    m_filepath(_filepath),
    m_max_file_size(_max_filesize < s_min_filesize ? s_min_filesize : _max_filesize)
{
}

void Logger::info(const std::string & _message)
{
    write(LogLevel::info, [_message](std::fstream & _stream)
    {
        _stream << _message;
    });
}

void Logger::warning(const std::string & _message)
{
    write(LogLevel::warning, [_message](std::fstream & _stream)
    {
        _stream << _message;
    });
}

void Logger::warning(const std::exception & _exception)
{
    write(LogLevel::warning, [_exception](std::fstream & _stream)
    {
        _stream << "An exception has occurred:" << _exception.what();
    });
}

void Logger::warning(const std::string & _message, const std::exception & _exception)
{
    write(LogLevel::warning, [_message, _exception](std::fstream & _stream)
    {
        _stream << _message << std::endl << "Exception message: " << _exception.what();
    });
}

void Logger::error(const std::string & _message)
{
    write(LogLevel::error, [_message](std::fstream & _stream)
    {
        _stream << _message;
    });
}

void Logger::error(const std::exception & _exception)
{
    write(LogLevel::error, [_exception](std::fstream & _stream)
    {
        _stream << "An exception has occurred:" << _exception.what();
    });
}

void Logger::error(const std::string & _message, const std::exception & _exception)
{
    write(LogLevel::error, [_message, _exception](std::fstream & _stream)
    {
        _stream << _message << std::endl << "Exception message: " << _exception.what();
    });
}

void Logger::write(LogLevel _level, std::function<void(std::fstream &)> _callback)
{
    if(m_filepath.empty() || _level < m_min_level)
    {
        return;
    }
    try
    {
        boost::posix_time::ptime time = boost::posix_time::microsec_clock::local_time();
        log_mutex.lock();
        prepareFile();
        std::fstream stream(m_filepath.string(), std::fstream::out | std::ios_base::app);
        stream << boost::posix_time::to_simple_string(time)<< " [" << _level << "]: ";
        _callback(stream);
        stream << std::endl;
    }
    catch(...)
    {
        log_mutex.unlock();
        return;
    }
    log_mutex.unlock();
}

void Logger::prepareFile()
{
    boost::system::error_code error;
    boost::filesystem::path parent_dir = m_filepath.parent_path();
    if(!boost::filesystem::exists(parent_dir))
    {
        boost::filesystem::create_directories(parent_dir, error);
        if(error)
        {
            throw LoggerException(error.message());
        }
    }
    if(!boost::filesystem::exists(m_filepath))
    {
        return;
    }
    boost::uintmax_t size = boost::filesystem::file_size(m_filepath, error);
    if(error)
    {
        throw LoggerException(error.message());
    }
    if(size < m_max_file_size)
    {
        return;
    }
    incrementFileVersion(m_filepath);
}

void Logger::incrementFileVersion(const boost::filesystem::path & _path)
{
    std::string ext = _path.extension().string().substr(1);
    bool is_num_ext = !ext.empty() &&
        std::find_if(ext.begin(), ext.end(), [](char symbol) {
            return symbol < '0' || symbol > '9';
        }) == ext.end();
    boost::filesystem::path new_path = _path;
    if(is_num_ext)
    {
        std::stringstream new_ext_stream;
        new_ext_stream << atoi(ext.c_str()) + 1;
        new_path.replace_extension(boost::filesystem::path(new_ext_stream.str()));
    }
    else
    {
        new_path += ".1";
    }
    if(boost::filesystem::exists(new_path))
    {
        incrementFileVersion(new_path);
    }
    if(boost::filesystem::exists(new_path))
    {
        throw LoggerException(std::string("Unable to move the log file [") + new_path.string() + "]");
    }
    boost::filesystem::rename(_path, new_path);
}
