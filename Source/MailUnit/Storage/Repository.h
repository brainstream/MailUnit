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
#include <boost/filesystem/path.hpp>
#include <MailUnit/OS.h>
#include <MailUnit/Storage/StorageException.h>
#include <MailUnit/Storage/Dsel.h>
#include <MailUnit/Storage/Email.h>

namespace MailUnit {
namespace Storage {

class Repository final
{
private:
    class Database;

public:
    Repository(const boost::filesystem::path & _storage_direcotiry);
    ~Repository();
    std::unique_ptr<RawEmail> createRawEmail();
    uint32_t storeEmail(const RawEmail & _raw_email);
    //void findEmails(Dsel::Expression & _query/* TODO: out???*/);

private:
    void initStorageDirectory();
    boost::filesystem::path makeNewFileName(const MailUnit::PathString & _base, bool _temp);

private:
    boost::filesystem::path m_storage_direcotiry;
    Database * mp_database;
}; // class Repository

} // namespace Storage
} // namespace MailUnit

#endif // __MU_STORAGE_REPOSITORY_H__
