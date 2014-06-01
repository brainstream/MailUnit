#include <iostream>
#include "SmtpController.h"

using namespace StubMTP;

SmtpController::SmtpController(boost::asio::io_service &_io_service) :
    SessionProvider(_io_service)
{
    std::cout << "New SMTP session has been started\n\n";
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
}

void SmtpController::onFail()
{
    std::cerr << "Error. TODO: description!\n\n";
}
