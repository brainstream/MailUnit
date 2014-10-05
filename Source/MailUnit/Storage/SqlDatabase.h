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

#ifndef __MU_STORAGE_SQLDATABASE_H__
#define __MU_STORAGE_SQLDATABASE_H__

#include <array>
#include <tuple>
#include <MailUnit/Storage/Database.h>

namespace MailUnit {
namespace Storage {

class SqlDatabase : public Database
{
public:
    void storeEmail(const Email & _email);
    std::unique_ptr<DBObjectSet> query(const std::string & _dsel_query);
}; // class SqlDatabase

} // namespace Storage
} // namespace MailUnit

#endif // __MU_STORAGE_SQLDATABASE_H__
