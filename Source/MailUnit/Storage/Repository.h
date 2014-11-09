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

#ifndef __MU_STORAGE_REPOSITORY_H__
#define __MU_STORAGE_REPOSITORY_H__

#include <memory>
#include <vector>
#include <ostream>
#include <boost/noncopyable.hpp>
#include <boost/filesystem/path.hpp>
#include <MailUnit/OS/FileSystem.h>
#include <MailUnit/Storage/StorageException.h>
#include <MailUnit/Storage/Email.h>

struct sqlite3;

namespace MailUnit {
namespace Storage {

class Repository final : private boost::noncopyable
{
public:
    Repository(const boost::filesystem::path & _storage_direcotiry);
    ~Repository();
    std::unique_ptr<RawEmail> createRawEmail();
    uint32_t storeEmail(RawEmail & _raw_email);
    void findEmails(const std::string & _edsl_query, std::vector<std::unique_ptr<Email> > & _result);

private:
    void initStorageDirectory();
    boost::filesystem::path makeNewFileName(const MailUnit::OS::PathString & _base, bool _temp);
    void prepareDatabase();
    uint32_t insertMessage(const Email & _email, const std::string & _data_id);
    void insertExchange(const Email & _email, uint32_t _message_id);
    void mapEdslToSqlWhere(const std::string & _edsl, std::ostream & _out);

private:
    boost::filesystem::path m_storage_direcotiry;
    sqlite3 * mp_sqlite;
}; // class Repository

} // namespace Storage
} // namespace MailUnit

#endif // __MU_STORAGE_REPOSITORY_H__
