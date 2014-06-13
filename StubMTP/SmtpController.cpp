#include <iostream>
#include <StubMTP/SmtpController.h>
#include <StubMTP/Email/Mime.h>
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
    std::shared_ptr<Email::Mime> mime = Email::parseMime(_message);
    // TODO: get data from mime
    std::cout << "Message has been recived: \n" <<
                 "\tID: " << mime->message_id << std::endl <<
                 "\tDate: " << *mime->date << std::endl <<
                 "\tFrom: " << mime->from << std::endl <<
                 "\tTo: " << mime->to << std::endl;
    if(!mime->cc.empty())
    {
        std::cout << "\tCC: " << mime->cc << std::endl;
    }
    if(!mime->bcc.empty())
    {
        std::cout << "\tBCC: " << mime->bcc << std::endl;
    }
    std::cout << std::endl << _message.data << std::endl << std::endl;
    std::cout.flush();
    app().log().info("Message received"); // TODO: more details
}

void SmtpController::onFail()
{
    std::cerr << "Receiving a message failed"; // TODO: #ifdef DEBUG...
    app().log().error("Receiving a message failed");
}
