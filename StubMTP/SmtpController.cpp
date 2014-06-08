#include <iostream>
#include <StubMTP/SmtpController.h>
#include <StubMTP/Logger.h>

using namespace StubMTP;

SmtpController::SmtpController(boost::asio::io_service &_io_service) :
    SessionProvider(_io_service)
{
    std::cout << "New SMTP session has been started\n\n";
    log().info("New SMTP session has been started");
}

void SmtpController::onMessageRecieved(const Smtp::Message & _message)
{
    std::cout << "Message has been recived: \n" <<
                 "\tFrom: " << _message.from << std::endl;
    for(const std::string & to: _message.to)
        std::cout << "\tTo: " << to << std::endl;
    for(const std::string & cc: _message.cc)
        std::cout << "\tCC: " << cc << std::endl;
    std::cout << std::endl << _message.body << std::endl << std::endl;
    std::cout.flush();
    log().info("Message received"); // TODO: info
}

void SmtpController::onFail()
{
    std::cerr << "Receiving a message failed"; // TODO: #ifdef DEBUG...
    log().error("Receiving a message failed");
}
