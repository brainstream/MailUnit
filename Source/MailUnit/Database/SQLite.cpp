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
#include <cstdio>
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

inline std::string prepareValueString(const std::string & _string)
{
    return boost::replace_all_copy(_string, "'", "''");
}

} // namespace

SQLite::SQLite(const boost::filesystem::path & _filepath)
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

void SQLite::save(const Email & _email)
{
    prepareDatabase();
    unsigned int message_id = insertMessage(_email);
    insertExchange(message_id, _email);
}

void SQLite::prepareDatabase()
{
    char * error = nullptr;
    int insert_result = sqlite3_exec(mp_sqlite, sql_init_db, nullptr, this, &error);
    if(SQLITE_OK != insert_result)
    {
        std::string er_string(error);
        sqlite3_free(error);
        throw DatabaseException(er_string);
    }
}

unsigned int SQLite::insertMessage(const Email & _email)
{
    std::stringstream sqlstream;
    sqlstream << "INSERT INTO \"Message\" (\"Subject\", \"Data\") VALUES('" <<
        prepareValueString(_email.subject()) << "', '" <<
        prepareValueString(_email.data()) << "');\n" <<
        "SELECT last_insert_rowid();";
    unsigned int message_id = ~0;
    char * error = nullptr;
    int insert_result = sqlite3_exec(mp_sqlite, sqlstream.str().c_str(),
        [](void * pmessage_id, int count, char ** values, char **) {
            if(count == 1) std::sscanf(values[0], "%u", pmessage_id);
            return 0;
        }, &message_id, &error);
    if(SQLITE_OK != insert_result)
    {
        std::string er_string(error);
        sqlite3_free(error);
        throw DatabaseException(er_string);
    }
    if(~0 == message_id)
    {
        throw DatabaseException("Unable to obtain the inserted object ID");
    }
    return message_id;
}

void SQLite::insertExchange(unsigned int _message_id, const Email & _email)
{
    std::vector<std::pair<Email::AddressType, const std::vector<std::string> *>> typed_addresses = {
        { Email::AddressType::From, &_email.fromAddresses() },
        { Email::AddressType::To,   &_email.toAddresses()   },
        { Email::AddressType::Cc,   &_email.ccAddresses()   },
        { Email::AddressType::Bcc,  &_email.bccAddresses()  }
    };
    std::stringstream sqlstream;
    for(auto & typed_address : typed_addresses)
    {
        for(auto & address : *typed_address.second)
        {
            sqlstream << "INSERT INTO \"Exchange\" (\"Message\", \"Mailbox\", \"Reason\") VALUES ("
                << _message_id << ", '" << prepareValueString(address) << "', "
                << static_cast<short>(typed_address.first) << ");\n";
        }
    }
    char * error = nullptr;
    int insert_result = sqlite3_exec(mp_sqlite, sqlstream.str().c_str(), nullptr, nullptr, &error);
    if(SQLITE_OK != insert_result)
    {
        std::string er_string(error);
        sqlite3_free(error);
        throw DatabaseException(er_string);
    }
}
