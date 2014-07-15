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
#include <MailUnit/SmtpController.h>
#include <MailUnit/Email.h>
#include <MailUnit/Application.h>
#include <MailUnit/Database/Email.h>

using namespace MailUnit;

SmtpController::SmtpController(boost::asio::io_service &_io_service) :
    SessionProvider(_io_service)
{
    std::cout << "New SMTP session has been started\n\n";
    app().log().info("New SMTP session has been started");
}

void SmtpController::onMessageRecieved(const Smtp::Message & _message)
{
    Email * email = new Email(_message);
    std::cout << "Message has been recived: \n" <<
        "\tSubject: " << email->subject() << std::endl <<
         "\tFrom:\n";
    for(const std::string & from : email->fromAddresses())
        std::cout << "\t\t" << from << std::endl;
    std::cout << "\tTo:\n";
    for(const std::string & to : email->toAddresses())
        std::cout << "\t\t" << to << std::endl;
    std::cout << "\tCC:\n";
    for(const std::string & cc : email->ccAddresses())
        std::cout << "\t\t" << cc << std::endl;
    std::cout << "\tBCC:\n";
    for(const std::string & bcc : email->bccAddresses())
        std::cout << "\t\t" << bcc << std::endl;
    // TODO: date
    Database::saveEmail(*email);
    delete email;
    std::cout.flush();
    app().log().info("Message received"); // TODO: more details
}

void SmtpController::onFail()
{
    std::cerr << "Receiving a message failed"; // TODO: #ifdef DEBUG...
    app().log().error("Receiving a message failed");
}
