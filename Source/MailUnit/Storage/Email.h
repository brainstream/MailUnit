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

#ifndef __MU_STORAGE_EMAIL_H__
#define __MU_STORAGE_EMAIL_H__

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <boost/optional.hpp>
#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>

namespace MailUnit {
namespace Storage {

class RawEmail : private boost::noncopyable
{
public:
    explicit RawEmail(const boost::filesystem::path & _data_file_path) :
        m_data_file_path(_data_file_path),
        m_data_out(_data_file_path.string())
    {
    }

    ~RawEmail()
    {
        m_data_out.close();
        boost::filesystem::remove(m_data_file_path);
    }

    std::ostream & data()
    {
        return m_data_out;
    }

    const boost::filesystem::path & dataFilePath() const
    {
        return m_data_file_path;
    }

    const std::vector<std::string> & fromAddresses() const
    {
        return m_from_addresses;
    }

    void addFromAddress(const std::string & _address)
    {
        m_from_addresses.push_back(_address);
    }

    const std::vector<std::string> & toAddresses() const
    {
        return m_to_addresses;
    }

    void addToAddress(const std::string & _address)
    {
        m_to_addresses.push_back(_address);
    }

private:
    boost::filesystem::path m_data_file_path;
    std::ofstream m_data_out;
    std::vector<std::string> m_from_addresses;
    std::vector<std::string> m_to_addresses;
}; // class RawEmail

class Email
{
public:
    enum class AddressType
    {
        From = 0,
        To   = 1,
        Cc   = 2,
        Bcc  = 3
    };

public:
    Email(const RawEmail & _raw, const boost::filesystem::path & _data_file_path);

    Email(const Email &) = default;

    Email & operator = (const Email &) = default;

    boost::optional<AddressType> findAddress(const std::string & _address);

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

private:
    void parseHeaders(int _file);
    void appendBcc(const RawEmail & _raw);

private:
    std::vector<std::string> m_from_addresses;
    std::vector<std::string> m_to_addresses;
    std::vector<std::string> m_cc_addresses;
    std::vector<std::string> m_bcc_addresses;
    std::string m_subject;
}; // class Email


} // namespace Storage
} // namespace MailUnit

#endif // __MU_STORAGE_EMAIL_H__
