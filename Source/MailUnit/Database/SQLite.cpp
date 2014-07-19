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

#include <sstream>
#include <boost/algorithm/string.hpp>
#include <MailUnit/Database/SQLite.h>

using namespace MailUnit::Data;

namespace {

constexpr const char * const sql_init_db =
    "CREATE TABLE IF NOT EXISTS \"Message\"(\n"
    "    \"Id\"      INTEGER PRIMARY KEY AUTOINCREMENT,\n"
    "    \"Subject\" TEXT,\n"
    "    \"Data\" TEXT\n"
    ");\n"
    "CREATE TABLE IF NOT EXISTS \"Exchange\"(\n"
    "    \"Id\"      INTEGER PRIMARY KEY AUTOINCREMENT,\n"
    "    \"Mailbox\" VARCHAR(250),\n"
    "    \"Message\" INTEGER,\n"
    "    \"Reason\"  INTEGER,\n"
    "    FOREIGN KEY(\"Message\") REFERENCES \"Message\"(\"Id\")\n"
    ");\n"
    "CREATE INDEX IF NOT EXISTS \"iMessageSubject\"  ON \"Message\"(\"Subject\");\n"
    "CREATE INDEX IF NOT EXISTS \"iExchangeMailbox\" ON \"Exchange\"(\"Mailbox\");\n"
    "CREATE INDEX IF NOT EXISTS \"iExchangeMessage\" ON \"Exchange\"(\"Message\");";

} // namespace

SQLite::SQLite(const boost::filesystem::path & _filepath) throw(DatabaseException)
{
    int sqlite_result = sqlite3_initialize();
    if(SQLITE_OK != sqlite_result)
        throw DatabaseException(sqlite3_errstr(sqlite_result));
#ifdef _WIN32
    sqlite_result = sqlite3_open16(_filepath.c_str(), &mp_sqlite);
#else
    sqlite_result = sqlite3_open(_filepath.c_str(), &mp_sqlite);
#endif
    if(SQLITE_OK != sqlite_result)
        throw DatabaseException(sqlite3_errstr(sqlite_result));
}

SQLite::~SQLite()
{
    if(nullptr != mp_sqlite)
    {
        sqlite3_close(mp_sqlite);
        mp_sqlite = nullptr;
    }
}

void SQLite::shutdown()
{
    sqlite3_shutdown();
}

void SQLite::save(const Email & _email) throw(DatabaseException)
{
    prepareDatabase();
    std::stringstream sqlstream;
    sqlstream << "INSERT INTO \"Message\" (\"Subject\", \"Data\") VALUES('" <<
        boost::replace_all_copy(_email.subject(), "'", "''") << "', '" <<
        boost::replace_all_copy(_email.data(), "'", "''") << "');";
    char * error = nullptr;
    sqlite3_exec(mp_sqlite, sqlstream.str().c_str(), nullptr, nullptr, &error);
    if(nullptr != error)
    {
        std::string er_string(error);
        sqlite3_free(error);
        throw DatabaseException(er_string);
    }
}

void SQLite::prepareDatabase() throw(DatabaseException)
{
    char * error = nullptr;
    sqlite3_exec(mp_sqlite, sql_init_db, nullptr, nullptr, &error);
    if(nullptr != error)
    {
        std::string er_string(error);
        sqlite3_free(error);
        throw DatabaseException(er_string);
    }
}
