#include <iostream>
#include <boost/asio.hpp>
#include <StubMTP/Smtp/Server.h>
#include <StubMTP/SmtpController.h>
#include <StubMTP/Application.h>

using namespace StubMTP;
using namespace StubMTP::Smtp;

namespace {

class PrivateApplication
{
public:
    PrivateApplication(int argc, const char ** argv);
    ~PrivateApplication();
    void start();
    inline Application & application() const;

private:
    Config * mp_config;
    Logger * mp_logger;
    Application * mp_application;
}; // class PrivateApplication

PrivateApplication * global_app = nullptr;

} // namespace


PrivateApplication::PrivateApplication(int _argc, const char ** _argv) :
    mp_config(nullptr),
    mp_logger(nullptr),
    mp_application(nullptr)
{
    try
    {
        mp_config = new Config(_argc, _argv);
        // TODO: use config isntead.
        // TODO: log to stdout
        mp_logger = new Logger(mp_config->logFilename(), mp_config->logLevel(), mp_config->logMaxSize());
        mp_application = new Application(*mp_config, *mp_logger);
    }
    catch(std::exception & err)
    {
        delete mp_config;
        delete mp_logger;
        delete mp_application;
        throw ApplicationException(err.what());
    }
}

PrivateApplication::~PrivateApplication()
{
    delete mp_application;
    delete mp_config;
    delete mp_logger;
}

void PrivateApplication::start()
{
    log().info("Application started");
    boost::asio::io_service service;
    Server::startNew(service, mp_config->portNumber(), std::make_shared<SmtpController>(service));
    service.run();
}

Application & PrivateApplication::application() const
{
    return *mp_application;
}

Application & StubMTP::application()
{
    if(nullptr == global_app)
        throw ApplicationException("Application wasn't instanced");
    return global_app->application();
}

int main(int _argc, const char ** _argv)
{
    try
    {
        global_app = new PrivateApplication(_argc, _argv);
        if(config().showHelp())
            std::cout << config() << std::endl;
        else
            global_app->start();
        delete global_app;
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
