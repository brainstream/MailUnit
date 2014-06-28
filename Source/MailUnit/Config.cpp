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

#include <MailUnit/Config.h>

#define LOPT_HELP              "help"
#define SOPT_HELP              "h"
#define LOPT_PORT              "port"
#define SOPT_PORT              "p"
#define LOPT_LOGSIZE           "lsize"
#define LOPT_LOGFILE           "lfile"
#define LOPT_LOGSTDOUT         "lstdout"
#define LOPT_LOGLEVEL_ERROR    "llerror"
#define LOPT_LOGLEVEL_WARNING  "llwarning"
#define LOPT_LOGLEVEL_INFO     "llinfo"
#define LOPT_THREADCOUNT       "threads"

using namespace MailUnit;
namespace bpo = boost::program_options;

Config::Config(int argc, const char ** argv) :
    m_log_max_size(Logger::s_defult_max_filesize),
    m_port_number(5870),
    m_thread_count(1),
    m_log_level(LogLevel::Info)
{
    mp_description = new bpo::options_description("Options");
    mp_description->add_options()
        (LOPT_HELP "," SOPT_HELP, "Print this help")
        (LOPT_PORT "," SOPT_PORT, bpo::value<uint16_t>(), "Port number")
        (LOPT_LOGSIZE, bpo::value<boost::uintmax_t>(), "Maximum size of each log file")
        (LOPT_LOGFILE, bpo::value<std::string>(), "Log filename")
        (LOPT_LOGSTDOUT, "Use stdout for logging")
        (LOPT_LOGLEVEL_ERROR, "Minimum log level: Error")
        (LOPT_LOGLEVEL_WARNING, "Minimum log level: Warning")
        (LOPT_LOGLEVEL_INFO, "Minimum log level: Info (default)")
        (LOPT_THREADCOUNT, bpo::value<uint16_t>(), "Thread count");
    bpo::variables_map var_map;
    bpo::store(bpo::parse_command_line(argc, argv, *mp_description), var_map);
    m_show_help = var_map.count(LOPT_HELP) > 0;
    m_log_to_stdout = var_map.count(LOPT_LOGSTDOUT) > 0;
    if(var_map.count(LOPT_LOGLEVEL_ERROR) > 0)
        m_log_level = LogLevel::Error;
    else if(var_map.count(LOPT_LOGLEVEL_WARNING) > 0)
        m_log_level = LogLevel::Warning;
    else
        m_log_level = LogLevel::Info;
    if(var_map.count(LOPT_THREADCOUNT) > 0)
    {
        m_thread_count = var_map[LOPT_THREADCOUNT].as<uint16_t>();
        if(m_thread_count == 0)
            m_thread_count = 1;
    }
    if(var_map.count(LOPT_PORT) > 0)
    {
        m_port_number = var_map[LOPT_PORT].as<uint16_t>();
    }
    if(var_map.count(LOPT_LOGSIZE) > 0)
    {
        m_log_max_size = var_map[LOPT_LOGSIZE].as<boost::uintmax_t>();
        if(m_log_max_size < Logger::s_min_filesize)
            m_log_max_size = Logger::s_min_filesize;
    }
    if(var_map.count(LOPT_LOGFILE) > 0)
    {
        m_log_filename = var_map[LOPT_LOGFILE].as<std::string>();
    }
}

Config::~Config()
{
    delete mp_description;
}

std::ostream & operator << (std::ostream & _stream, const Config & _config)
{
    _stream << *_config.mp_description;
    return _stream;
}
