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
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <boost/date_time.hpp>
#include <MailUnit/Logger.h>

using namespace MailUnit;

namespace {

std::mutex log_mutex;

inline void writeStream(std::ostream & _stream, const boost::posix_time::ptime & _time,
    LogLevel _level, const std::string & _message)
{
    _stream << boost::posix_time::to_simple_string(_time) <<
        " [" << _level << "]: " << _message << std::endl;
}

} // namespace


std::ostream & operator << (std::ostream & _stream, LogLevel _level)
{
    switch(_level)
    {
    case LogLevel::debug:
        _stream << "DEBUG";
        break;
    case LogLevel::info:
        _stream << "INFO";
        break;
    case LogLevel::warning:
        _stream << "WARNING";
        break;
    case LogLevel::error:
        _stream << "ERROR";
        break;
    case LogLevel::fatal:
        _stream << "FATAL";
        break;
    }
    return _stream;
}

Logger::Logger(const Options & _options) :
    m_options(_options)
{
    if(m_options.max_filesize < min_filesize)
        m_options.max_filesize = min_filesize;
}

void Logger::write(LogLevel _level, const std::string & _message)
{
    if(_level < m_options.min_level || (!m_options.stdlog && m_options.filepath.empty()))
    {
        return;
    }
    boost::posix_time::ptime time = boost::posix_time::microsec_clock::local_time();
    if(m_options.stdlog)
    {
        writeStream(std::clog, time, _level, _message);
    }
    if(!m_options.filepath.empty())
    {
        try
        {
            log_mutex.lock();
            prepareFile();
            std::fstream stream(m_options.filepath.string(), std::fstream::out | std::ios_base::app);
            writeStream(stream, time, _level, _message);
        }
        catch(...)
        {
            log_mutex.unlock();
            return;
        }
        log_mutex.unlock();
    }
}

void Logger::prepareFile()
{
    boost::system::error_code error;
    boost::filesystem::path parent_dir = m_options.filepath.parent_path();
    if(!boost::filesystem::exists(parent_dir))
    {
        boost::filesystem::create_directories(parent_dir, error);
        if(error)
        {
            throw LoggerException(error.message());
        }
    }
    if(!boost::filesystem::exists(m_options.filepath))
    {
        return;
    }
    boost::uintmax_t size = boost::filesystem::file_size(m_options.filepath, error);
    if(error)
    {
        throw LoggerException(error.message());
    }
    if(size < m_options.max_filesize)
    {
        return;
    }
    incrementFileVersion(m_options.filepath);
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
