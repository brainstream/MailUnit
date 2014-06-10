#include <iostream>
#include <StubMTP/SmtpController.h>
#include <StubMTP/Application.h>

using namespace StubMTP;

SmtpController::SmtpController(boost::asio::io_service &_io_service) :
    SessionProvider(_io_service)
{
    std::cout << "New SMTP session has been started\n\n";
    app().log().info("New SMTP session has been started");
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
    app().log().info("Message received"); // TODO: info
}

void SmtpController::onFail()
{
    std::cerr << "Receiving a message failed"; // TODO: #ifdef DEBUG...
    app().log().error("Receiving a message failed");
}
