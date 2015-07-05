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

#include <vector>
#include <set>
#include <string>
#include <fstream>
#include <ctime>
#include <boost/optional.hpp>
#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>
#include <Include/LibMailUnit/Def.h>
#include <MailUnit/String.h>
#include <MailUnit/Storage/StorageException.h>

namespace MailUnit {
namespace Storage {

class RawEmail : private boost::noncopyable
{
public:
    explicit RawEmail(const boost::filesystem::path & _data_file_path) :
        m_data_file_path(_data_file_path),
        m_data_out(_data_file_path.string(), std::ios_base::binary)
    {
    }

    ~RawEmail()
    {
        m_data_out.close();
        boost::filesystem::remove(m_data_file_path);
    }

    std::ostream & data()
    {
        if(!m_data_out.is_open())
            throw StorageException("Email storage file is not open");
        return m_data_out;
    }

    void flush()
    {
        if(m_data_out.is_open())
            m_data_out.flush();
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
    typedef std::set<std::string, StringLessICompare> AddressSet;

    enum class AddressType : short
    {
        from = 0,
        to   = 1,
        cc   = 2,
        bcc  = 3
    };

    static const uint32_t new_object_id = static_cast<uint32_t>(-1);

public:
    Email(uint32_t _id, const boost::filesystem::path & _data_file_path, bool _parse_file);

    Email(const RawEmail & _raw, const boost::filesystem::path & _data_file_path);

    Email(const Email &) = default;

    Email & operator = (const Email &) = default;

    uint32_t id() const
    {
        return m_id;
    }

    boost::optional<AddressType> findAddress(const std::string & _address);

    bool containsAddress(const std::string & _address, AddressType _type) const;

    const AddressSet & addresses(AddressType _type) const;

    bool addAddress(AddressType _type, const std::string & _address);

    const std::string & subject() const
    {
        return m_subject;
    }

    void setSubject(const std::string & _subject)
    {
        m_subject = _subject;
    }

    const boost::filesystem::path & dataFilePath() const
    {
        return m_data_file_path;
    }

    std::time_t sendingTime() const
    {
        return m_sending_time;
    }

    void setSendingTime(std::time_t _date_time)
    {
        m_sending_time = _date_time;
    }

private:
    void parseHeaders(MU_NATIVE_FILE _file);
    void appendFrom(const RawEmail & _raw);
    void appendBcc(const RawEmail & _raw);

private:
    uint32_t m_id;
    boost::filesystem::path  m_data_file_path;
    AddressSet m_from_addresses;
    AddressSet m_to_addresses;
    AddressSet m_cc_addresses;
    AddressSet m_bcc_addresses;
    std::string m_subject;
    std::time_t m_sending_time;
}; // class Email


} // namespace Storage
} // namespace MailUnit

#endif // __MU_STORAGE_EMAIL_H__
