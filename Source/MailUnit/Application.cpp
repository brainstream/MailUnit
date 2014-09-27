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

#include <iostream>
#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>
#include <MailUnit/Server/TcpServer.h>
#include <MailUnit/Smtp/ServerRequestHandler.h>
#include <MailUnit/Mqp/ServerRequestHandler.h>
#include <MailUnit/Storage/SQLite/SQLite.h>
#include <MailUnit/Application.h>

using namespace MailUnit;
using namespace MailUnit::Server;

namespace {

class PrivateApplication : public Application
{
public:
    inline PrivateApplication(int _argc, const char ** _argv);
    void start();
}; // class PrivateApplication

PrivateApplication * global_app = nullptr;

} // namespace


Application::Application(int _argc, const char ** _argv) :
    mp_config(nullptr),
    mp_logger(nullptr),
    mr_start_dir(boost::filesystem::initial_path())
{
    try
    {
        mp_config = new Config(_argc, _argv);
        // TODO: use config isntead.
        // TODO: log to stdout
        mp_logger = new Logger(mp_config->logFilename(), mp_config->logLevel(), mp_config->logMaxSize());
    }
    catch(std::exception & err)
    {
        delete mp_config;
        delete mp_logger;
        throw ApplicationException(err.what());
    }
}

Application::~Application()
{
    delete mp_config;
    delete mp_logger;
}

PrivateApplication::PrivateApplication(int _argc, const char ** _argv) :
    Application(_argc, _argv)
{
}

void PrivateApplication::start()
{
    app().log().info("Application started");

    boost::asio::io_service service;

    // TODO: config
    std::shared_ptr<Storage::Database> database(new Storage::SQLite("/home/brainstream/temp/mailunit.sqlite"));
    // TODO: ip address from config
    boost::asio::ip::tcp::endpoint smtp_server_endpoint(boost::asio::ip::tcp::v4(), config().portNumber());
    startTcpServer(service, smtp_server_endpoint, std::make_shared<Smtp::ServerRequestHandler>(database));

    // TODO: from config (including ip address)
    boost::asio::ip::tcp::endpoint storage_server_endpoint(boost::asio::ip::tcp::v4(), 5880);
    startTcpServer(service, storage_server_endpoint, std::make_shared<Mqp::ServerRequestHandler>(database));

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
    try
    {
        boost::scoped_ptr<PrivateApplication> app(new PrivateApplication(_argc, _argv));
        global_app = app.get();
        if(app->config().showHelp())
            std::cout << app->config() << std::endl;
        else
            app->start();
        // TODO: finalizer with the SQLite::shutdownd call
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
