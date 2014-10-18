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
#include <cstdint>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/locale/encoding.hpp>
#include <SQLite/sqlite3.h>
#include <MailUnit/Application.h>
#include <MailUnit/Storage/Repository.h>
#include <MailUnit/Storage/Edsl.h>

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
        inline MailUnit::PathString genFilename()
        {
            return uuidToPathString(m_generator());
        }

        inline boost::uuids::uuid genUuid()
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
        return unique_id_generator.genFilename();
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

    inline std::string formatSqliteError(const std::string & _message, int _error)
    {
        std::stringstream result;
        result << _message << std::endl <<
                  "SQLite error number: " << _error << std::endl <<
                  "SQLite error message: " << sqlite3_errstr(_error);
        return result.str();
    }

    inline Email * reverseFindEmail(std::vector<std::unique_ptr<Email>> & _emails, uint32_t _id)
    {
        auto it = std::find_if(_emails.rbegin(), _emails.rend(), [_id](const auto & email) {
            return email->id() == _id;
        });
        return _emails.rend() == it ? nullptr : it->get();
    }
} // namespace


Repository::Repository(const fs::path & _storage_direcotiry) :
    m_storage_direcotiry(_storage_direcotiry.is_relative() ?
        fs::absolute(_storage_direcotiry, MailUnit::app().startDir()) : _storage_direcotiry)
{
    initStorageDirectory();
    std::string db_utf8_filepath = getUtf8Filename(makeNewFileName(db_filename, false));
    int result = sqlite3_open(db_utf8_filepath.c_str(), &mp_sqlite);
    if(SQLITE_OK != result)
    {
        throw StorageException(formatSqliteError("Unable to connect to the SQLite database", result));
    }
    prepareDatabase();
}

Repository::~Repository()
{
    sqlite3_close(mp_sqlite);
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

void Repository::prepareDatabase()
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

std::unique_ptr<RawEmail> Repository::createRawEmail()
{
    return std::make_unique<RawEmail>(makeNewFileName(generateUniqueFilename(), true));
}

uint32_t Repository::storeEmail(const RawEmail & _raw_email)
{
    boost::uuids::uuid data_id = unique_id_generator.genUuid();
    fs::path data_filepath = makeNewFileName(unique_id_generator.uuidToPathString(data_id), false);
    boost::scoped_ptr<Email> email(new Email(_raw_email, data_filepath));
    uint32_t message_id = insertMessage(*email, boost::uuids::to_string(data_id));
    insertExchange(*email, message_id);
    return message_id;
}

uint32_t Repository::insertMessage(const Email & _email, const std::string & _data_id)
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

void Repository::insertExchange(const Email & _email, uint32_t _message_id)
{
    Email::AddressType address_types[] = {
        Email::AddressType::From,
        Email::AddressType::To,
        Email::AddressType::Cc,
        Email::AddressType::Bcc
    };
    std::stringstream sql;
    for(Email::AddressType address_type : address_types)
    {
        const Email::AddressSet & address_set = _email.addresses(address_type);
        for(const std::string & address : address_set)
        {
            sql << "INSERT INTO Exchange (Message, Mailbox, Reason) VALUES (" <<
                   _message_id << ", '" << prepareSqlValueString(address) << "', " <<
                   static_cast<short>(address_type) << ");\n";
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

void Repository::findEmails(const std::string & _edsl_query, std::vector<std::unique_ptr<Email>> & _result)
{
    std::string query = boost::algorithm::trim_copy(_edsl_query);
    std::stringstream sql;
    sql << "SELECT m.Id, m.DataId, m.Subject, e.Reason, e.Mailbox "
           "FROM Message m "
           "inner join Exchange e ON e.Message = m.Id";
    if(!query.empty())
    {
        // TODO: append the WHERE part
    }
    struct CallbackArgs
    {
        Repository * repository;
        std::vector<std::unique_ptr<Email>> * result;
    } callback_args = { this, &_result };
    char * error = nullptr;
    int insert_result = sqlite3_exec(mp_sqlite, sql.str().c_str(),
        [](void * pargs, int, char ** values, char **) {
            CallbackArgs * args = static_cast<CallbackArgs *>(pargs);
            uint32_t id = boost::lexical_cast<uint32_t>(values[0]);
            Email * email = reverseFindEmail(*args->result, id);
            if(nullptr == email)
            {
                std::string data_id = values[1];
                email = new Email(id, args->repository->makeNewFileName(data_id, false), false);
                args->result->push_back(std::unique_ptr<Email>(email));
                email->setSubject(values[2]);
            }
            email->addAddress(static_cast<Email::AddressType>(boost::lexical_cast<short>(values[3])), values[4]);
            return 0;
        }, &callback_args, &error);
    if(SQLITE_OK != insert_result)
    {
        std::string er_string("Unable to select e-mail objects:\n");
        er_string += error;
        sqlite3_free(error);
        throw StorageException(formatSqliteError(er_string, insert_result));
    }
}
