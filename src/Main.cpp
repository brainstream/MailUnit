#include <boost/asio.hpp>
#include "Smtp/Server.h"
#include "SmtpController.h"

using namespace StubMTP;
using namespace StubMTP::Smtp;

int main()
{
    boost::asio::io_service service;
    Server::startNew(service, 17444, std::make_shared<SmtpController>(service));
    service.run();
    return 0;
}
