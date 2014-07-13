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

#ifndef __MU_EMAIL_H__
#define __MU_EMAIL_H__

#include <vector>
#include <string>
#include <MailUnit/Smtp/Message.h>

namespace MailUnit {

class Email
{
public:
    enum class AddressType
    {
        Invalid,
        From,
        To,
        Cc,
        Bcc
    };

public:
    Email(const Smtp::Message & _smtp_message);
    AddressType containsAddress(const std::string & _address);

public:
    const std::vector<std::string> & fromAddresses() const
    {
        return m_from_addresses;
    }

    const std::vector<std::string> & toAddresses() const
    {
        return m_to_addresses;
    }

    const std::vector<std::string> & ccAddresses() const
    {
        return m_cc_addresses;
    }

    const std::vector<std::string> & bccAddresses() const
    {
        return m_bcc_addresses;
    }

    const std::string & subject() const
    {
        return m_subject;
    }

    const std::string & data() const
    {
        return m_data;
    }

private:
    void parseSmtpHeaders(const Smtp::Message & _smtp_message);
    void appendBccFromSmtpMessage(const Smtp::Message & _smtp_message);

private:
    std::vector<std::string> m_from_addresses;
    std::vector<std::string> m_to_addresses;
    std::vector<std::string> m_cc_addresses;
    std::vector<std::string> m_bcc_addresses;
    std::string m_subject;
    std::string m_data;
}; // class Email

} // namespace MailUnit

#endif // __MU_EMAIL_H__
