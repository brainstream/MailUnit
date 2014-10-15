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
#include <locale>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/locale/encoding.hpp>
#include <SQLite/sqlite3.h>
#include <MailUnit/Application.h>
#include <MailUnit/Storage/Repository.h>

using namespace MailUnit::Storage;
namespace fs = boost::filesystem;

namespace {
    static const MailUnit::PathString tmp_file_ext = MU_PATHSTR(".tmp");
    static const MailUnit::PathString db_filename = MU_PATHSTR("index.db");
    static const char * const sql_init_db =
        "CREATE TABLE IF NOT EXISTS Message(\n"
        "    Id      INTEGER PRIMARY KEY AUTOINCREMENT,\n"
        "    DataId  VARCHAR(36),\n"
        "    Subject TEXT\n"
        ");\n"
        "CREATE TABLE IF NOT EXISTS Exchange(\n"
        "    Id      INTEGER PRIMARY KEY AUTOINCREMENT,\n"
        "    Mailbox VARCHAR(250),\n"
        "    Message INTEGER,\n"
        "    Reason  INTEGER,\n"
        "    FOREIGN KEY(Message) REFERENCES Message(Id)\n"
        ");\n"
        "CREATE INDEX IF NOT EXISTS iMessageSubject  ON Message(Subject);\n"
        "CREATE INDEX IF NOT EXISTS iExchangeMailbox ON Exchange(Mailbox);\n"
        "CREATE INDEX IF NOT EXISTS iExchangeMessage ON Exchange(Message);";

    thread_local static class
    {
    public:
        inline MailUnit::PathString generateUniqueFilename()
        {
            return uuidToPathString(m_generator());
        }

        inline boost::uuids::uuid generateUuid()
        {
            return m_generator();
        }

        inline MailUnit::PathString uuidToPathString(const boost::uuids::uuid & _uuid) const
        {
#ifdef MU_PATHISWIDECHAR
            return boost::uuids::to_wstring(_uuid);
#else
            return boost::uuids::to_string(_uuid);
#endif
        }

    private:
        boost::uuids::random_generator m_generator;
    } unique_id_generator;

    inline MailUnit::PathString generateUniqueFilename()
    {
        return unique_id_generator.generateUniqueFilename();
    }

    inline std::string getUtf8Filename(const fs::path & _path)
    {
#ifdef MU_PATHISWIDECHAR
        return boost::locale::conv::utf_to_utf<char>(_path.string());
#else
        return _path.string();
#endif
    }

    inline std::string prepareSqlValueString(const std::string & _string)
    {
        return boost::replace_all_copy(_string, "'", "''");
    }
} // namespace

class Repository::Database final : private boost::noncopyable
{
public:
    explicit Database(const fs::path & _db_file_path);
    ~Database();
    uint32_t insertMessage(const Email & _email, const std::string & _data_id);
    void insertExchange(const Email & _email, uint32_t _message_id);

private:
    inline std::string formatSqliteError(const std::string & _message, int _error);
    void prepareDatabase();

private:
    sqlite3 * mp_sqlite;
}; // class Repository::Database


Repository::Repository(const fs::path & _storage_direcotiry) :
    m_storage_direcotiry(_storage_direcotiry.is_relative() ?
        fs::absolute(_storage_direcotiry, MailUnit::app().startDir()) : _storage_direcotiry)
{
    mp_database = new Database(makeNewFileName(db_filename, false));
    initStorageDirectory();
}

Repository::~Repository()
{
    delete mp_database;
}

void Repository::initStorageDirectory()
{
    try
    {
        fs::create_directories(m_storage_direcotiry);
    }
    catch(const fs::filesystem_error & error)
    {
        std::stringstream message;
        message << "An error occurred during a storage direcorty initialization." << std::endl <<
                   "Path: " << error.path1() << std::endl <<
                   "Error: " << error.what();
        throw(StorageException(message.str()));
    }
}

boost::filesystem::path Repository::makeNewFileName(const MailUnit::PathString & _base, bool _temp)
{
    return m_storage_direcotiry / boost::filesystem::path(_temp ? _base + tmp_file_ext : _base);
}

std::unique_ptr<RawEmail> Repository::createRawEmail()
{
    return std::make_unique<RawEmail>(makeNewFileName(generateUniqueFilename(), true));
}

uint32_t Repository::storeEmail(const RawEmail & _raw_email)
{
    boost::uuids::uuid data_id = unique_id_generator.generateUuid();
    fs::path data_filepath = makeNewFileName(unique_id_generator.uuidToPathString(data_id), false);
    boost::scoped_ptr<Email> email(new Email(_raw_email, data_filepath));
    uint32_t message_id = mp_database->insertMessage(*email, boost::uuids::to_string(data_id));
    mp_database->insertExchange(*email, message_id);
    return message_id;
}

Repository::Database::Database(const fs::path & _db_file_path)
{
    int result = sqlite3_open(getUtf8Filename(_db_file_path).c_str(), &mp_sqlite);
    if(SQLITE_OK != result)
    {
        throw StorageException(formatSqliteError("Unable to connect to the SQLite database", result));
    }
    prepareDatabase();
}

Repository::Database::~Database()
{
    sqlite3_close(mp_sqlite);
}

std::string Repository::Database::formatSqliteError(const std::string & _message, int _error)
{
    std::stringstream result;
    result << _message << std::endl <<
              "SQLite error number: " << _error << std::endl <<
              "SQLite error message: " << sqlite3_errstr(_error);
}

void Repository::Database::prepareDatabase()
{
    char * error = nullptr;
    int insert_result = sqlite3_exec(mp_sqlite, sql_init_db, nullptr, nullptr, &error);
    if(SQLITE_OK != insert_result)
    {
        std::string er_string("Unable to initialize SQLite database:\n");
        er_string += error;
        sqlite3_free(error);
        throw StorageException(formatSqliteError(er_string, insert_result));
    }
}

uint32_t Repository::Database::insertMessage(const Email & _email, const std::string & _data_id)
{
    std::stringstream sql;
    sql << "INSERT INTO Message (Subject, DataId) VALUES ('" <<
                          prepareSqlValueString(_email.subject()) << "','" << _data_id <<
                          "');\nSELECT last_insert_rowid();";
    uint32_t message_id;
    char * error = nullptr;
    int insert_result = sqlite3_exec(mp_sqlite, sql.str().c_str(),
        [](void * pmessage_id, int count, char ** values, char **) {
            if(count == 1)
            {
                *static_cast<uint32_t *>(pmessage_id) = boost::lexical_cast<unsigned int>(values[0]);
            }
            return 0;
        }, &message_id, &error);
    if(SQLITE_OK != insert_result)
    {
        std::string er_string("Unable to store an e-mail object:\n");
        er_string += error;
        sqlite3_free(error);
        throw StorageException(formatSqliteError(er_string, insert_result));
    }
    return message_id;
}

void Repository::Database::insertExchange(const Email & _email, uint32_t _message_id)
{
    std::vector<std::pair<Email::AddressType, const std::vector<std::string> *>> typed_addresses = {
        { Email::AddressType::From, &_email.fromAddresses() },
        { Email::AddressType::To,   &_email.toAddresses()   },
        { Email::AddressType::Cc,   &_email.ccAddresses()   },
        { Email::AddressType::Bcc,  &_email.bccAddresses()  }
    };
    std::stringstream sql;
    for(auto & typed_address : typed_addresses)
    {
        for(auto & address : *typed_address.second)
        {
            sql << "INSERT INTO Exchange (Message, Mailbox, Reason) VALUES (" <<
                   _message_id << ", '" << prepareSqlValueString(address) << "', " <<
                   static_cast<short>(typed_address.first) << ");\n";
        }
    }
    char * error = nullptr;
    int insert_result = sqlite3_exec(mp_sqlite, sql.str().c_str(), nullptr, nullptr, &error);
    if(SQLITE_OK != insert_result)
    {
        std::string er_string("Unable to store an exchange object:\n");
        er_string += error;
        sqlite3_free(error);
        throw StorageException(formatSqliteError(er_string, insert_result));
    }
}
