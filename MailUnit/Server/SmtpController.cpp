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
#include <MailUnit/Server/SmtpController.h>
#include <MailUnit/Server/Email/Mime.h>
#include <MailUnit/Server/Application.h>

using namespace MailUnit;

SmtpController::SmtpController(boost::asio::io_service &_io_service) :
    SessionProvider(_io_service)
{
    std::cout << "New SMTP session has been started\n\n";
    app().log().info("New SMTP session has been started");
}

void SmtpController::onMessageRecieved(const Smtp::Message & _message)
{
    Email::Mime * mime = new Email::Mime(_message);
    std::cout << "Message has been recived: \n";
    {
        std::cout << "\tID: ";
        Email::MessageIdPtr message_id = mime->messageId();
        if(nullptr != message_id)
        {
            std::cout << message_id << " (left: " << message_id->left() <<
                         ", right: " << message_id->right() << ")";
        }
        std::cout << std::endl;
    }
    std::cout << "\tSubject: " << mime->subject() << std::endl <<
                 "\tDate: " << *mime->date() << std::endl <<
                 "\tFrom: " << mime->from() << std::endl <<
                 "\tTo: " << mime->to() << std::endl <<
                 "\tCC: " << mime->cc() << std::endl <<
                 "\tBCC: " << mime->bcc() << std::endl;
    delete mime;
    std::cout << std::endl << _message.data << std::endl << std::endl;
    std::cout.flush();
    app().log().info("Message received"); // TODO: more details
}

void SmtpController::onFail()
{
    std::cerr << "Receiving a message failed"; // TODO: #ifdef DEBUG...
    app().log().error("Receiving a message failed");
}
