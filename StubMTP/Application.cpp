#include <iostream>
#include <boost/asio.hpp>
#include <StubMTP/Smtp/Server.h>
#include <StubMTP/SmtpController.h>
#include <StubMTP/Application.h>

using namespace StubMTP;
using namespace StubMTP::Smtp;

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
    Server::startNew(service, config().portNumber(), std::make_shared<SmtpController>(service));
    service.run();
}



Application & StubMTP::app()
{
    if(nullptr == global_app)
        throw ApplicationException("Application wasn't instanced");
    return *global_app;
}

int main(int _argc, const char ** _argv)
{
    try
    {
        global_app = new PrivateApplication(_argc, _argv);
        if(global_app->config().showHelp())
            std::cout << global_app->config() << std::endl;
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
