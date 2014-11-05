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

#include <cstdlib>
#include <boost/asio.hpp>
#include <boost/variant.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <MailUnit/Def.h>
#include <MailUnit/DeferredPointer.h>
#include <MailUnit/OS/FileSystem.h>
#include <MailUnit/Server/TcpServer.h>
#include <MailUnit/Smtp/ServerRequestHandler.h>
#include <MailUnit/Mqp/ServerRequestHandler.h>

#define LOPT_HELP         "help"
#define SOPT_HELP         "h"
#define LOPT_SMTP_PORT    "smtp-port"
#define SOPT_SMTP_PORT    "s"
#define LOPT_MQP_PORT     "mqp-port"
#define SOPT_MQP_PORT     "m"
#define SOPT_STORAGE_DIR  "d"
#define LOPT_STORAGE_DIR  "storage-dir"
#define LOPT_LOGSIZE      "log-size"
#define LOPT_LOGFILE      "log-file"
#define LOPT_STDLOG       "log-std"
#define LOPT_LOGLEVEL     "log-level"

#define LOG_LEVEL_INFO    "info"
#define LOG_LEVEL_WARNING "warning"
#define LOG_LEVEL_ERROR   "error"

using namespace MailUnit;
using namespace MailUnit::Storage;
using namespace MailUnit::OS;
namespace po = boost::program_options;
namespace fs = boost::filesystem;
namespace asio = boost::asio;

namespace {

DeferredPointer<Logger> deferred_logger_pointer;

} // namespace

namespace MailUnit {

Logger * const logger = deferred_logger_pointer.unsafeGet();

void validate(boost::any & _out_value, const std::vector<std::string> & _in_values, LogLevel *, int)
{
    po::validators::check_first_occurrence(_out_value);
    const std::string & input = po::validators::get_single_string(_in_values);
    if(boost::iequals(input, LOG_LEVEL_ERROR))
        _out_value = boost::any(LogLevel::error);
    else if(boost::iequals(input, LOG_LEVEL_WARNING))
        _out_value = boost::any(LogLevel::warning);
    else if(boost::iequals(input, LOG_LEVEL_INFO))
        _out_value = boost::any(LogLevel::info);
    else
        throw po::validation_error(po::validation_error::invalid_option_value);
}

} // namespace MailUnit

namespace {


struct Config
{
    bool use_stdlog;
    boost::uintmax_t log_max_size;
    fs::path log_filepath;
    fs::path data_dirpath;
    uint16_t smtp_port;
    uint16_t mqp_port;
    uint16_t thread_count;
    LogLevel log_level;
}; // struct Config

typedef boost::variant<std::shared_ptr<po::options_description>, std::shared_ptr<Config>> OptionsPtr;

struct LoadConfigError
{
    std::string message;
    std::shared_ptr<po::options_description> options_description;
}; // struct LoadConfigError

fs::path configFile(const fs::path & _dir)
{
    if(_dir.empty())
        return fs::path();
    fs::path file = _dir / BOOST_STRINGIZE(_MU_BINARY_NAME) ".conf";
    return fs::exists(file) ? file : fs::path();
}

OptionsPtr loadConfig(int _argc, const char ** _argv, const fs::path & _app_dir)
{
    std::shared_ptr<Config> config(new Config);
    std::shared_ptr<po::options_description> full_description(new po::options_description);
    po::options_description cmd_line_only_description("Command line only options");
    po::options_description common_description("Options");

    PathString log_file;
    PathString data_dir;
    cmd_line_only_description.add_options()
        (LOPT_HELP "," SOPT_HELP, "Print this help");
    common_description.add_options()
        (LOPT_SMTP_PORT "," SOPT_SMTP_PORT, po::value(&config->smtp_port)->required(),
            "SMTP server port number")
        (LOPT_MQP_PORT "," SOPT_MQP_PORT, po::value(&config->mqp_port)->required(),
            "MQP server port number")
        (LOPT_STORAGE_DIR "," SOPT_STORAGE_DIR, po::value(&data_dir)->required(),
            "Data storage directory")
        (LOPT_LOGSIZE, po::value(&config->thread_count)->default_value(1),
            "Maximum size of each log file")
        (LOPT_LOGFILE, po::value(&log_file), "Log filename")
        (LOPT_STDLOG, "Use stdlog")
        (LOPT_LOGLEVEL,
            po::value(&config->log_level)->default_value(LogLevel::error, LOG_LEVEL_ERROR),
            "Log level. Valid values: " LOG_LEVEL_INFO ", " LOG_LEVEL_WARNING ", " LOG_LEVEL_ERROR);
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
        return full_description;
    }
    try
    {
        po::notify(var_map);
    }
    catch(std::exception & error)
    {
        throw LoadConfigError { error.what(), full_description };
    }
    config->use_stdlog = var_map.count(LOPT_STDLOG) > 0;
    if(!log_file.empty())
        config->log_filepath = toAbsolutePath(log_file, _app_dir);
    if(!data_dir.empty())
        config->data_dirpath = toAbsolutePath(data_dir, _app_dir);
    return config;
}

void printUsage(const po::options_description & _options_description, std::ostream & _stream)
{
    _stream << "Usage: " BOOST_PP_STRINGIZE(_MU_BINARY_NAME) << " [options]" <<
        std::endl << _options_description << std::endl;
}

void start(const std::shared_ptr<Config> _config)
{
    deferred_logger_pointer.construct(_config->log_filepath, _config->log_level, _config->log_max_size);
    logger->info("Application started");

    asio::io_service service;

    std::shared_ptr<Repository> repo = std::make_shared<Repository>(_config->data_dirpath);
    // TODO: interface from config
    asio::ip::tcp::endpoint smtp_server_endpoint(asio::ip::tcp::v4(), _config->smtp_port);
    startTcpServer(service, smtp_server_endpoint, std::make_shared<Smtp::ServerRequestHandler>(repo));

    // TODO: interface from config
    asio::ip::tcp::endpoint storage_server_endpoint(asio::ip::tcp::v4(), _config->mqp_port);
    startTcpServer(service, storage_server_endpoint, std::make_shared<Mqp::ServerRequestHandler>(repo));

    service.run();
}

struct Runner : boost::static_visitor<>
{
    void operator ()(const std::shared_ptr<po::options_description> _option_description)
    {
        printUsage(*_option_description, std::cout);
    }

    void operator ()(const std::shared_ptr<Config> _config)
    {
        start(_config);
    }
}; // struct Runner

} // namespace

int main(int _argc, const char ** _argv)
{
    try
    {
        OptionsPtr opts = loadConfig(_argc, _argv, fs::initial_path());
        Runner runner;
        boost::apply_visitor(runner, opts);
        return EXIT_SUCCESS;
    }
    catch(const LoadConfigError & error)
    {
        std::cerr << error.message << std::endl << std::endl;
        printUsage(*error.options_description, std::cerr);
        return EXIT_FAILURE;
    }
    catch(const std::exception & error)
    {
        std::cerr << error.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch(...)
    {
        std::cerr << "Unknown error has occurred" << std::endl;
        return EXIT_FAILURE;
    }
}
