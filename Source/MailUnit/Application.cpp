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

#if !defined __cplusplus || __cplusplus < 201300L
#   error This project requires a C++14 compatible compiler!
#endif // __cplusplus && __cplusplus >= 201300L

#include <iostream>
#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <MailUnit/Server/TcpServer.h>
#include <MailUnit/Smtp/ServerRequestHandler.h>
#include <MailUnit/Mqp/ServerRequestHandler.h>
#include <MailUnit/Application.h>

using namespace MailUnit;
using namespace MailUnit::Server;

namespace {

class PrivateApplication : public Application
{
public:
    explicit PrivateApplication(std::shared_ptr<const Config> _config) :
        Application(_config)
    {
    }

    void start();
}; // class PrivateApplication

void printUsage(const ConfigLoader & _config_loader)
{
    std::cout << "Usage: " BOOST_PP_STRINGIZE(_MU_BINARY_NAME) << " [options]" <<
        std::endl << std::endl << _config_loader << std::endl;
}

std::shared_ptr<const Config> processConfig(int _argc, const char ** _argv,
    const boost::filesystem::path & _app_dir)
{
    boost::scoped_ptr<ConfigLoader> config_loader;
    try
    {
        std::shared_ptr<const Config> config;
        config_loader.reset(new ConfigLoader(_argc, _argv, _app_dir));
        config = config_loader->config();
        if(config->show_help)
        {
            printUsage(*config_loader);
            return nullptr;
        }
        config_loader->flush();
        return config;
    }
    catch(std::exception & error)
    {
        std::cerr << error.what() << std::endl << std::endl;
        if(config_loader)
            printUsage(*config_loader);
        throw;
    }
}

PrivateApplication * global_app = nullptr;

} // namespace


Application::Application(std::shared_ptr<const Config> _config) :
    m_config_ptr(_config),
    mp_logger(nullptr)
{
    try
    {
        // TODO: log to stdout
        mp_logger = new Logger(m_config_ptr->log_filepath,
            m_config_ptr->log_level, m_config_ptr->log_max_size);
    }
    catch(std::exception & err)
    {
        delete mp_logger;
        throw ApplicationException(err.what());
    }
}

Application::~Application()
{
    delete mp_logger;
}

void PrivateApplication::start()
{
    app().log().info("Application started");

    boost::asio::io_service service;

    // TODO: config
    std::shared_ptr<Storage::Repository> repo = std::make_shared<Storage::Repository>(
            #ifdef _WIN32
                L"D:\\temp\\mailunit\\"
            #else
                "/home/brainstream/temp/mailunit/"
            #endif
                );
    // TODO: interface from config
    boost::asio::ip::tcp::endpoint smtp_server_endpoint(boost::asio::ip::tcp::v4(), config().smtp_port);
    startTcpServer(service, smtp_server_endpoint, std::make_shared<Smtp::ServerRequestHandler>(repo));

    // TODO: interface from config
    boost::asio::ip::tcp::endpoint storage_server_endpoint(boost::asio::ip::tcp::v4(), config().mqp_port);
    startTcpServer(service, storage_server_endpoint, std::make_shared<Mqp::ServerRequestHandler>(repo));

    service.run();
}

Application & MailUnit::app()
{
    if(nullptr == global_app)
        throw ApplicationException("Application wasn't instanced");
    return *global_app;
}


int main(int _argc, const char ** _argv)
{
    std::shared_ptr<const Config> config;
    try
    {
        config = processConfig(_argc, _argv, boost::filesystem::initial_path());
        if(nullptr == config)
            return EXIT_SUCCESS;
    }
    catch(...)
    {
        return EXIT_FAILURE;
    }
    try
    {
        boost::scoped_ptr<PrivateApplication> app(new PrivateApplication(config));
        global_app = app.get();
        app->start();
        return EXIT_SUCCESS;
    }
    catch(std::exception & error)
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
