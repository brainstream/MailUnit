#include <iostream>
#include <boost/asio.hpp>
#include <StubMTP/Config.h>
#include <StubMTP/Logger.h>
#include <StubMTP/Smtp/Server.h>
#include <StubMTP/SmtpController.h>

using namespace StubMTP;
using namespace StubMTP::Smtp;

int main(int argc, const char ** argv)
{
    Config * config;
    try
    {
        config = new Config(argc, argv);
    }
    catch(const std::exception & error)
    {
        std::cerr << error.what() << std::endl;
        return 1;
    }
    if(config->showHelp())
    {
        std::cout << *config << std::endl;
        return 0;
    }
    Logger::initSingleton("StubMTP.log", LogLevel::Info);
    log().info("Application started");
    boost::asio::io_service service;
    Server::startNew(service, config->portNumber(), std::make_shared<SmtpController>(service));
    service.run();

    // TODO: to the program final
    delete config;

    return 0;
}

