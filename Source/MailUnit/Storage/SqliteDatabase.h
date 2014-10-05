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

#ifndef __MU_STORAGE_SQLITEDATABASE_H__
#define __MU_STORAGE_SQLITEDATABASE_H__

#include <MailUnit/Storage/SqlDatabase.h>

namespace MailUnit {
namespace Storage {

class SqliteDatabase : public SqlDatabase
{
public:
    SqliteDatabase(const std::string & _filename)
    {
        // TODO: implement!
        (void)_filename;
    }

}; // class SqliteDatabase

} // namespace Storage
} // namespace MailUnit

#endif // __MU_STORAGE_SQLITEDATABASE_H__
