#include <boost/asio.hpp>
#include <StubMTP/Smtp/Server.h>
#include <StubMTP/SmtpController.h>
#include <StubMTP/Logger.h>

using namespace StubMTP;
using namespace StubMTP::Smtp;

int main()
{
    Logger::initSingleton("StubMTP.log", LogLevel::Info);
    log().info("Application started");
    boost::asio::io_service service;
    Server::startNew(service, 17444, std::make_shared<SmtpController>(service));
    service.run();
    return 0;
}
