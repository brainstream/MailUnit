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

#ifndef __cplusplus
#	error The C++ compiler is required!
#elif defined(_MSC_VER) && (_MSC_VER < 1900)
#	error Microsoft Visual Studio 2015 or greater is required!
#elif !defined(_MSC_VER) && __cplusplus < 201300L
#   error A C++14 compatible compiler is required!
#endif

#include <thread>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/filesystem.hpp>
#include <MailUnit/Config.h>
#include <MailUnit/Logger.h>
#include <MailUnit/DeferredPointer.h>
#include <MailUnit/Server/Tcp/TcpServer.h>
#include <MailUnit/Smtp/ServerRequestHandler.h>
#include <MailUnit/Mqp/ServerRequestHandler.h>

using namespace MailUnit;
using namespace MailUnit::Storage;
namespace po = boost::program_options;
namespace fs = boost::filesystem;
namespace asio = boost::asio;

namespace {

DeferredPointer<Logger> deferred_logger_pointer;

} // namespace

namespace MailUnit {

Logger * const logger = deferred_logger_pointer.unsafeGet();

} // namespace MailUnit

namespace {

void printUsage(const po::options_description & _options_description, std::ostream & _stream)
{
    _stream << "Usage: " BOOST_PP_STRINGIZE(_MU_BINARY_NAME) << " [options]" <<
        std::endl << _options_description << std::endl;
}

void start(const std::shared_ptr<Config> _config)
{
    Logger::Options logger_options;
    logger_options.filepath = _config->log_filepath;
    logger_options.max_filesize = _config->log_max_size;
    logger_options.min_level = _config->log_level;
    logger_options.stdlog = _config->use_stdlog;
    deferred_logger_pointer.construct(logger_options);

    LOG_INFO << "Application started";

    asio::io_service service;

    std::shared_ptr<Repository> repo = std::make_shared<Repository>(_config->data_dirpath);
    // TODO: interface from config
    asio::ip::tcp::endpoint smtp_server_endpoint(asio::ip::tcp::v4(), _config->smtp_port);
    startTcpServer(service, smtp_server_endpoint, std::make_shared<Smtp::ServerRequestHandler>(repo, *_config));

    // TODO: interface from config
    //asio::ip::tcp::endpoint storage_server_endpoint(asio::ip::address_v4::from_string("0.0.0.0"), _config->mqp_port);
    asio::ip::tcp::endpoint storage_server_endpoint(asio::ip::tcp::v4(), _config->mqp_port);
    startTcpServer(service, storage_server_endpoint, std::make_shared<Mqp::ServerRequestHandler>(repo));

    uint16_t thread_count = _config->thread_count;
    if(thread_count < MU_MIN_THREAD_COUNT) thread_count = MU_MIN_THREAD_COUNT;
    else if(thread_count > MU_MAX_THREAD_COUNT) thread_count = MU_MAX_THREAD_COUNT;

    std::thread * threads = new std::thread[thread_count];
    for(uint16_t i = 0; i < thread_count; ++i)
    {
        threads[i] = std::thread([&service]() {
            service.run();
        });
    }
    asio::signal_set sigs(service, SIGINT, SIGTERM);
    sigs.async_wait([&service](const boost::system::error_code &, int) {
        LOG_INFO << "Stopping application...";
        service.stop();
    });
    for(uint16_t i = 0; i < thread_count; ++i)
    {
        threads[i].join();
    }
    delete [] threads;
}

} // namespace

int main(int _argc, const char ** _argv)
{
    try
    {
        loadConfig(_argc, _argv, fs::initial_path(), start,
            std::bind(printUsage, std::placeholders::_1, std::ref(std::cout)));
        return EXIT_SUCCESS;
    }
    catch(const ConfigLoadingException & error)
    {
        std::cerr << error.what() << std::endl << std::endl;
        printUsage(error.optionsDescription(), std::cerr);
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
