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

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/program_options.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/algorithm/string.hpp>
#include <MailUnit/Config.h>
#include <MailUnit/OS/FileSystem.h>

#define LOPT_HELP            "help"
#define SOPT_HELP            "h"
#define LOPT_SMTP_PORT       "smtp-port"
#define SOPT_SMTP_PORT       "s"
#define LOPT_SMTP_STARTTLS   "smtp-starttls"
#define LOPT_SMTP_CERT       "smtp-cert"
#define LOPT_SMTP_PKEY       "smtp-pkey"
#define LOPT_SMTP_PKEYPASS   "smtp-pkeypass"
#define LOPT_MQP_PORT        "mqp-port"
#define SOPT_MQP_PORT        "m"
#define SOPT_STORAGE_DIR     "d"
#define LOPT_STORAGE_DIR     "storage-dir"
#define SOPT_THREAD_COUTN    "t"
#define LOPT_THREAD_COUTN    "threads"
#define LOPT_LOGSIZE         "log-size"
#define LOPT_LOGFILE         "log-file"
#define LOPT_STDLOG          "log-std"
#define LOPT_LOGLEVEL        "log-level"

#define LOG_LEVEL_DEBUG      "debug"
#define LOG_LEVEL_INFO       "info"
#define LOG_LEVEL_WARNING    "warning"
#define LOG_LEVEL_ERROR      "error"
#define LOG_LEVEL_FATAL      "fatal"

using namespace MailUnit;
using namespace MailUnit::OS;
namespace po = boost::program_options;
namespace fs = boost::filesystem;

namespace {

fs::path configFile(const fs::path & _dir)
{
    if(_dir.empty())
        return fs::path();
    fs::path file = _dir / BOOST_STRINGIZE(_MU_CONFIG_DIRECTORY) ".conf";
    return fs::exists(file) ? file : fs::path();
}

} // namespace

namespace MailUnit {

void validate(boost::any & _out_value, const std::vector<std::string> & _in_values, LogLevel *, int)
{
    po::validators::check_first_occurrence(_out_value);
    const std::string & input = po::validators::get_single_string(_in_values);
    if(boost::iequals(input, LOG_LEVEL_FATAL))
        _out_value = boost::any(LogLevel::fatal);
    if(boost::iequals(input, LOG_LEVEL_ERROR))
        _out_value = boost::any(LogLevel::error);
    else if(boost::iequals(input, LOG_LEVEL_WARNING))
        _out_value = boost::any(LogLevel::warning);
    else if(boost::iequals(input, LOG_LEVEL_INFO))
        _out_value = boost::any(LogLevel::info);
    else if(boost::iequals(input, LOG_LEVEL_DEBUG))
        _out_value = boost::any(LogLevel::debug);
    else
        throw po::validation_error(po::validation_error::invalid_option_value);
}

} // namespace MailUnit

void MailUnit::loadConfig(int _argc, const char ** _argv, const boost::filesystem::path & _app_dir,
    ConfigLoadedCallback _config_loaded_callback, PrintUsageCallback _print_usage_callback)
{
    std::shared_ptr<Config> config(new Config);
    std::shared_ptr<po::options_description> full_description(new po::options_description);
    po::options_description cmd_line_only_description("Command line only options");
    po::options_description common_description("Options");
    PathString log_file;
    PathString data_dir;
    PathString smtp_cert_path;
    PathString smtp_pkey_path;
    const boost::uintmax_t defult_max_filesize = Logger::defult_max_filesize;
    cmd_line_only_description.add_options()
        (LOPT_HELP "," SOPT_HELP, "Print this help.");
    common_description.add_options()
        (LOPT_SMTP_PORT "," SOPT_SMTP_PORT, po::value(&config->smtp_port)->required(),
            "SMTP server port number.")
        (LOPT_SMTP_STARTTLS,
            "Allow STARTTLS SMTP extenstion. If this options specified, " LOPT_SMTP_CERT " and " LOPT_SMTP_PKEY " options are required.")
        (LOPT_SMTP_CERT, po::value(&smtp_cert_path), "Path to the certificate file. Only PEM format is allowed.")
        (LOPT_SMTP_PKEY, po::value(&smtp_pkey_path), "Path to the private key file. Only PEM format is allowed.")
        (LOPT_SMTP_PKEYPASS, po::value(&config->smtp_privet_key_pass), "Password for decryption the private key.")

        (LOPT_MQP_PORT "," SOPT_MQP_PORT, po::value(&config->mqp_port)->required(),
            "MQP server port number.")
        (LOPT_STORAGE_DIR "," SOPT_STORAGE_DIR, po::value(&data_dir)->required(),
            "Data storage directory.")
        (LOPT_THREAD_COUTN "," SOPT_THREAD_COUTN, po::value(&config->thread_count)->default_value(MU_MIN_THREAD_COUNT),
            "Working thread count (" BOOST_PP_STRINGIZE(MU_MIN_THREAD_COUNT) " – "  BOOST_PP_STRINGIZE(MU_MAX_THREAD_COUNT) ")" )
        (LOPT_LOGSIZE, po::value(&config->log_max_size)->default_value(defult_max_filesize),
            "Maximum size of each log file in bytes.")
        (LOPT_LOGFILE, po::value(&log_file), "Log filename.")
        (LOPT_STDLOG, "Use stdlog.")
        (LOPT_LOGLEVEL,
            po::value(&config->log_level)->default_value(LogLevel::error, LOG_LEVEL_ERROR),
            "Log level. \nValid values: "
            LOG_LEVEL_DEBUG ", " LOG_LEVEL_INFO ", " LOG_LEVEL_WARNING ", " LOG_LEVEL_ERROR ", " LOG_LEVEL_FATAL ".");
    full_description->add(common_description).add(cmd_line_only_description);

    po::variables_map var_map;
    po::store(po::parse_command_line(_argc, _argv, *full_description), var_map);
    for(const fs::path & path : { _app_dir, userConfigDirectory(), systemConfigDirectory() })
    {
        fs::path config_file = configFile(path);
        if(!config_file.empty())
        {
            fs::fstream stream(config_file, std::ios_base::in);
            po::store(po::parse_config_file(stream, common_description), var_map);
        }
    }
    if(var_map.count(LOPT_HELP) > 0)
    {
        _print_usage_callback(*full_description);
        return;
    }
    try
    {
        po::notify(var_map);
    }
    catch(std::exception & error)
    {
        throw ConfigLoadingException(error.what(), full_description);
    }
    config->use_smtp_starttls = var_map.count(LOPT_SMTP_STARTTLS) > 0;
    if(!smtp_cert_path.empty())
        config->smtp_cert_path = toAbsolutePath(smtp_cert_path, _app_dir);
    if(!smtp_pkey_path.empty())
        config->smtp_privet_key_path = toAbsolutePath(smtp_pkey_path, _app_dir);
    if(config->use_smtp_starttls && (config->smtp_cert_path.empty() || config->smtp_privet_key_path.empty()))
    {
        throw ConfigLoadingException("The certificate and private key are required to use SSL/TLS", full_description);
    }
    config->use_stdlog = var_map.count(LOPT_STDLOG) > 0;
    if(!log_file.empty())
        config->log_filepath = toAbsolutePath(log_file, _app_dir);
    if(!data_dir.empty())
        config->data_dirpath = toAbsolutePath(data_dir, _app_dir);
    _config_loaded_callback(config);
}
