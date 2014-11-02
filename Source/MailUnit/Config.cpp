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

#include <istream>
#include <boost/algorithm/string.hpp>
#include <MailUnit/Config.h>
#include <MailUnit/OS/FileSystem.h>

#define LOPT_HELP         "help"
#define SOPT_HELP         "h"
#define LOPT_SMTP_PORT    "smtp-port"
#define SOPT_SMTP_PORT    "s"
#define LOPT_MQP_PORT     "mqp-port"
#define SOPT_MQP_PORT     "m"
#define LOPT_LOGSIZE      "log-size"
#define LOPT_LOGFILE      "log-file"
#define LOPT_STDLOG       "log-std"
#define LOPT_LOGLEVEL     "log-level"

#define LOG_LEVEL_INFO    "info"
#define LOG_LEVEL_WARNING "warning"
#define LOG_LEVEL_ERROR   "error"

using namespace MailUnit;
namespace bpo = boost::program_options;

namespace MailUnit {

void validate(boost::any & _out_value, const std::vector<std::string> & _in_values, LogLevel *, int)
{
    bpo::validators::check_first_occurrence(_out_value);
    const std::string & input = bpo::validators::get_single_string(_in_values);
    if(boost::iequals(input, LOG_LEVEL_ERROR))
        _out_value = boost::any(LogLevel::error);
    else if(boost::iequals(input, LOG_LEVEL_WARNING))
        _out_value = boost::any(LogLevel::warning);
    else if(boost::iequals(input, LOG_LEVEL_INFO))
        _out_value = boost::any(LogLevel::info);
    else
        throw bpo::validation_error(bpo::validation_error::invalid_option_value);
}

} // namespace MailUnit

ConfigLoader::ConfigLoader(int _argc, const char ** _argv, const boost::filesystem::path & _app_dir) :
    m_config_ptr(new Config),
    mp_var_map(new bpo::variables_map),
    mp_cmd_line_description(new bpo::options_description("Options"))
{
    MailUnit::OS::PathString log_file;
    mp_cmd_line_description->add_options()
        (LOPT_HELP "," SOPT_HELP, "Print this help")
        (LOPT_SMTP_PORT "," SOPT_SMTP_PORT, bpo::value(&m_config_ptr->smtp_port)->required(),
            "SMTP server port number")
        (LOPT_MQP_PORT "," SOPT_MQP_PORT, bpo::value(&m_config_ptr->mqp_port)->required(),
            "MQP server port number")
        (LOPT_LOGSIZE, bpo::value(&m_config_ptr->thread_count)->default_value(1),
            "Maximum size of each log file")
        (LOPT_LOGFILE, bpo::value(&log_file), "Log filename")
        (LOPT_STDLOG, "Use stdlog")
        (LOPT_LOGLEVEL,
            bpo::value(&m_config_ptr->log_level)->default_value(LogLevel::error, LOG_LEVEL_ERROR),
            "Log level. Valid values: " LOG_LEVEL_INFO ", " LOG_LEVEL_WARNING ", " LOG_LEVEL_ERROR);
    bpo::store(bpo::parse_command_line(_argc, _argv, *mp_cmd_line_description), *mp_var_map);
    m_config_ptr->show_help = mp_var_map->count(LOPT_HELP) > 0;
    m_config_ptr->use_stdlog = mp_var_map->count(LOPT_STDLOG) > 0;

    if(!log_file.empty())
    {
        m_config_ptr->log_filepath = log_file;
        if(m_config_ptr->log_filepath.is_relative())
            m_config_ptr->log_filepath = _app_dir / m_config_ptr->log_filepath;
    }
    m_config_ptr->app_dir = _app_dir;
}

ConfigLoader::~ConfigLoader()
{
    delete mp_cmd_line_description;
    delete mp_var_map;
}
