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
#include <type_traits>
#include <MailUnit/EnumMap.h>
#include <MailUnit/Storage/Database.h>

namespace MailUnit {
namespace Storage {
namespace SqlSchema {

enum class MessageTable
{
    Column_Id,
    Column_Subject,
    Column_Data
}; // enum class MessageTableSchema

enum class ExchangeTable
{
    Column_Id,
    Column_Mailbox,
    Column_Message,
    Column_Reason
}; // enum class ExchangeTableSchema

} // namespace SqlSchema
} // namespace Storage
} // namespace MailUnit

BEGIN_ENUM_MAP(MailUnit::Storage::SqlSchema::MessageTable)
    MAP_ENUM_ITEM(MailUnit::Storage::SqlSchema::MessageTable::Column_Id,      "Id")
    MAP_ENUM_ITEM(MailUnit::Storage::SqlSchema::MessageTable::Column_Subject, "Subject")
    MAP_ENUM_ITEM(MailUnit::Storage::SqlSchema::MessageTable::Column_Data,    "Data")
END_ENUM_MAP

BEGIN_ENUM_MAP(MailUnit::Storage::SqlSchema::ExchangeTable)
    MAP_ENUM_ITEM(MailUnit::Storage::SqlSchema::ExchangeTable::Column_Id,      "Id")
    MAP_ENUM_ITEM(MailUnit::Storage::SqlSchema::ExchangeTable::Column_Mailbox, "Mailbox")
    MAP_ENUM_ITEM(MailUnit::Storage::SqlSchema::ExchangeTable::Column_Message, "Message")
    MAP_ENUM_ITEM(MailUnit::Storage::SqlSchema::ExchangeTable::Column_Reason,  "Reason")
END_ENUM_MAP

#define SQL_COLUMN_NAME(column) ENUM_ITEM_NAME(column)

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
