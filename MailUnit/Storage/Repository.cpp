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
#include <MailUnit/Storage/Repository.h>
#include <MailUnit/Storage/Edsl.h>

using namespace MailUnit::Storage;
namespace fs = boost::filesystem;

namespace {

static const MailUnit::OS::PathString tmp_file_ext = MU_PATHSTR(".tmp");
static const MailUnit::OS::PathString db_filename = MU_PATHSTR("index.db");

namespace TableMessage {
static const std::string table_name     = "Message";
static const std::string column_id      = "Id";
static const std::string column_data_id = "DataId";
static const std::string column_subject = "Subject";
} // namespace TableMessage

namespace TableExchange {
static const std::string table_name     = "Exchange";
static const std::string column_id      = "Id";
static const std::string column_mailbox = "Mailbox";
static const std::string column_message = "Message";
static const std::string column_reason  = "Reason";
} // namespace TableExchange

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

class EdsToSqlMapper : public boost::static_visitor<>
{
public:
    explicit EdsToSqlMapper(std::ostream & _sql) :
        mr_sql(_sql)
    {
    }

    void mapToSqlWhereClause(const Edsl::ConditionSequence & _sequence);

    void operator ()(const Edsl::BinaryCondition & _bin_condition);

    void operator ()(const Edsl::ConditionSequence & _sequence)
    {
        mr_sql << " (";
        mapToSqlWhereClause(_sequence);
        mr_sql << ") ";
    }

private:
    void addMailboxCause(Edsl::ConditionBinaryOperator _operator,
        Email::AddressType _address_type, const std::string & _address);

private:
    std::ostream & mr_sql;
}; // class EdsToSqlMapper

void EdsToSqlMapper::mapToSqlWhereClause(const Edsl::ConditionSequence & _sequence)
{
    boost::apply_visitor(*this, _sequence.operand);
    for(const Edsl::RightConditionSequence & right : _sequence.right)
    {
        switch(right.operator_)
        {
        case Edsl::ConditionJoinOperator::and_:
            mr_sql << " AND ";
            break;
        case Edsl::ConditionJoinOperator::or_:
            mr_sql << " OR ";
            break;
        }
        boost::apply_visitor(*this, right.operand);
    }
}

void EdsToSqlMapper::operator ()(const Edsl::BinaryCondition & _bin_condition)
{
    if(boost::algorithm::iequals("FROM", _bin_condition.identifier))
    {
        addMailboxCause(_bin_condition.operator_, Email::AddressType::from,
            boost::get<std::string>(_bin_condition.value));
    }
    else if(boost::algorithm::iequals("TO", _bin_condition.identifier))
    {
        addMailboxCause(_bin_condition.operator_, Email::AddressType::to,
            boost::get<std::string>(_bin_condition.value));
    }
    else if(boost::algorithm::iequals("CC", _bin_condition.identifier))
    {
        addMailboxCause(_bin_condition.operator_, Email::AddressType::cc,
            boost::get<std::string>(_bin_condition.value));
    }
    else if(boost::algorithm::iequals("BCC", _bin_condition.identifier))
    {
        addMailboxCause(_bin_condition.operator_, Email::AddressType::bcc,
            boost::get<std::string>(_bin_condition.value));
    }
    else if(boost::algorithm::iequals("SUBJECT", _bin_condition.identifier))
    {
        mr_sql << TableMessage::table_name << '.' << TableMessage::column_subject;
        if(_bin_condition.operator_ == Edsl::ConditionBinaryOperator::equal)
        {
            mr_sql << " = '";
        }
        else if(_bin_condition.operator_ == Edsl::ConditionBinaryOperator::not_equal)
        {
            mr_sql << " <> '";
        }
        else
        {
            std::stringstream message;
            message << '"' << _bin_condition.operator_ << "\" is not supported operator for Subject causes";
            throw StorageException(message.str());
        }
        mr_sql << boost::get<std::string>(_bin_condition.value) << "' ";
    }
}

void EdsToSqlMapper::addMailboxCause(Edsl::ConditionBinaryOperator _operator,
    Email::AddressType _address_type, const std::string & _address)
{
    if(_operator == Edsl::ConditionBinaryOperator::equal)
    {
        mr_sql << " (";
    }
    else if(_operator == Edsl::ConditionBinaryOperator::not_equal)
    {
        mr_sql << " ( NOT (";
    }
    else
    {
        std::stringstream message;
        message << '"' << _operator << "\" is not supported operator for Mailbox causes";
        throw StorageException(message.str());
    }
    mr_sql <<
        TableExchange::table_name << '.' << TableExchange::column_reason << " = " <<
        static_cast<uint16_t>(_address_type) << " AND " <<
        TableExchange::table_name << '.' << TableExchange::column_mailbox << " = '" << _address;
    if(_operator == Edsl::ConditionBinaryOperator::equal)
    {
        mr_sql << "') ";
    }
    else if(_operator == Edsl::ConditionBinaryOperator::not_equal)
    {
        mr_sql << "')) ";
    }
}

thread_local static class
{
public:
    inline MailUnit::OS::PathString genFilename()
    {
        return uuidToPathString(m_generator());
    }

    inline boost::uuids::uuid genUuid()
    {
        return m_generator();
    }

    inline MailUnit::OS::PathString uuidToPathString(const boost::uuids::uuid & _uuid) const
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

inline MailUnit::OS::PathString generateUniqueFilename()
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

inline Email * reverseFindEmail(std::vector<std::unique_ptr<Email>> & _emails, uint32_t _id)
{
    auto it = std::find_if(_emails.rbegin(), _emails.rend(), [_id](const auto & email) {
        return email->id() == _id;
    });
    return _emails.rend() == it ? nullptr : it->get();
}

} // namespace


Repository::Repository(const fs::path & _storage_direcotiry) :
    m_storage_direcotiry(_storage_direcotiry)
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

boost::filesystem::path Repository::makeNewFileName(const MailUnit::OS::PathString & _base, bool _temp)
{
    return m_storage_direcotiry / boost::filesystem::path(_temp ? _base + tmp_file_ext : _base);
}

void Repository::prepareDatabase()
{
    std::stringstream sql;
    sql <<
        "CREATE TABLE IF NOT EXISTS " << TableMessage::table_name << "(\n" <<
        TableMessage::column_id <<  " INTEGER PRIMARY KEY AUTOINCREMENT,\n" <<
        TableMessage::column_data_id << " VARCHAR(36),\n" <<
        TableMessage::column_subject << " TEXT\n);\n" <<

        "CREATE TABLE IF NOT EXISTS " << TableExchange::table_name << "(\n" <<
        TableExchange::column_id << " INTEGER PRIMARY KEY AUTOINCREMENT,\n" <<
        TableExchange::column_mailbox <<  " VARCHAR(250),\n" <<
        TableExchange::column_message << " INTEGER,\n" <<
        TableExchange::column_reason << " INTEGER,\n" <<
        "FOREIGN KEY(" << TableExchange::column_message << ") REFERENCES " <<
        TableMessage::table_name << "(" << TableMessage::column_id << ")\n);\n" <<

        "CREATE INDEX IF NOT EXISTS iMessageSubject ON " << TableMessage::table_name <<
        "(" << TableMessage::column_subject << ");\n" <<
        "CREATE INDEX IF NOT EXISTS iExchangeMailbox ON " << TableExchange::table_name <<
        "(" << TableExchange::column_mailbox << ");\n" <<
        "CREATE INDEX IF NOT EXISTS iExchangeMessage ON " << TableExchange::table_name <<
        "(" << TableExchange::column_message << ");";
    char * error = nullptr;
    int insert_result = sqlite3_exec(mp_sqlite, sql.str().c_str(), nullptr, nullptr, &error);
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

uint32_t Repository::storeEmail(RawEmail & _raw_email)
{
    _raw_email.flush();
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
    sql << "INSERT INTO " << TableMessage::table_name << " (" <<
        TableMessage::column_subject << ", " << TableMessage::column_data_id << ") VALUES ('" <<
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
        Email::AddressType::from,
        Email::AddressType::to,
        Email::AddressType::cc,
        Email::AddressType::bcc
    };
    std::stringstream sql;
    for(Email::AddressType address_type : address_types)
    {
        const Email::AddressSet & address_set = _email.addresses(address_type);
        for(const std::string & address : address_set)
        {
            sql << "INSERT INTO " << TableExchange::table_name << "  (" <<
                TableExchange::column_message << ", " <<
                TableExchange::column_mailbox << ", " <<
                TableExchange::column_reason << ") VALUES (" <<
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

std::shared_ptr<QueryResult> Repository::executeQuery(const std::string & _edsl_query)
{
    std::string query = boost::algorithm::trim_copy(_edsl_query);
    std::unique_ptr<Edsl::Expression> expression;
    try
    {
        expression = Edsl::parse(query);
    }
    catch(Edsl::EdslException & _error)
    {
        auto result =  makeQueryResult<QueryErrorResult>();
        boost::get<QueryErrorResult>(*result).message = _error.what();
        return result;
    }
    try
    {
        if(expression->operation == Edsl::Operation::get)
        {
            auto result =  makeQueryResult<QueryGetResult>();
            findEmails(*expression, boost::get<QueryGetResult>(*result).emails);
            return result;
        }
        if(expression->operation == Edsl::Operation::drop)
        {
            auto result =  makeQueryResult<QueryDropResult>();
            boost::get<QueryDropResult>(*result).count = dropEmails(*expression);
            return result;
        }
    }
    catch(...)
    {
        auto result =  makeQueryResult<QueryErrorResult>();
        boost::get<QueryErrorResult>(*result).message = "Server error has occurred"; // TODO: more details!
        return result;
    }
    auto result =  makeQueryResult<QueryErrorResult>();
    boost::get<QueryErrorResult>(*result).message = "Unknown query operation";
    return result;
}

void Repository::findEmails(const Edsl::Expression & _expression, std::vector<std::unique_ptr<Email>> & _result)
{
    std::stringstream sql;
    sql << "SELECT " <<
           TableMessage::table_name  << '.' << TableMessage::column_id       << ',' <<
           TableMessage::table_name  << '.' << TableMessage::column_data_id  << ',' <<
           TableMessage::table_name  << '.' << TableMessage::column_subject  << ',' <<
           TableExchange::table_name << '.' << TableExchange::column_reason  << ',' <<
           TableExchange::table_name << '.' << TableExchange::column_mailbox <<
           " FROM " << TableMessage::table_name <<
           " INNER JOIN " <<TableExchange::table_name << " ON " <<
           TableExchange::table_name << '.' << TableExchange::column_message << '=' <<
           TableMessage::table_name << '.' << TableMessage::column_id << std::endl;
    mapEdslToSqlSelectWhere(_expression, sql);
    struct CallbackArgs
    {
        Repository * repository;
        std::vector<std::unique_ptr<Email>> * result;
    } callback_args = { this, &_result };
    char * error = nullptr;
#ifdef DBGLOG
    std::cout << "SELECT SQL: " << sql.str() << std::endl;
#endif
    int select_result = sqlite3_exec(mp_sqlite, sql.str().c_str(),
        [](void * pargs, int, char ** values, char **) {
            CallbackArgs * args = static_cast<CallbackArgs *>(pargs);
            uint32_t id = boost::lexical_cast<uint32_t>(values[0]);
            Email * email = reverseFindEmail(*args->result, id);
            if(nullptr == email)
            {
                MailUnit::OS::PathString data_id = MailUnit::OS::utf8ToPathString(values[1]);
                email = new Email(id, args->repository->makeNewFileName(data_id, false), false);
                args->result->push_back(std::unique_ptr<Email>(email));
                email->setSubject(values[2]);
            }
            email->addAddress(static_cast<Email::AddressType>(boost::lexical_cast<short>(values[3])), values[4]);
            return 0;
        }, &callback_args, &error);
    if(SQLITE_OK != select_result)
    {
        std::string er_string("Unable to select e-mail objects:\n");
        er_string += error;
        sqlite3_free(error);
        throw StorageException(formatSqliteError(er_string, select_result));
    }
}

size_t Repository::dropEmails(const Edsl::Expression & _expression)
{
    boost::scoped_ptr<std::vector<std::unique_ptr<Email>>> emails(new std::vector<std::unique_ptr<Email>>());
    findEmails(_expression, *emails);
    if(emails->empty())
        return 0;
    std::string sql;
    {
        std::stringstream exchange_sql;
        std::stringstream message_sql;
        exchange_sql << "DELETE FROM " << TableExchange::table_name <<
               " WHERE " << TableExchange::column_message << " IN (";
        message_sql << "DELETE FROM " << TableMessage::table_name <<
                " WHERE " << TableMessage::column_id << " IN (";
        bool first_email = true;
        for(const std::unique_ptr<Email> & email : *emails)
        {
            if(first_email)
            {
                first_email = false;
            }
            else
            {
                exchange_sql << ", ";
                message_sql << ", ";
            }
            exchange_sql << email->id();
            message_sql << email->id();
        }
        exchange_sql << ");";
        message_sql << ")";
        exchange_sql << message_sql.str();
        sql = exchange_sql.str();
    }

#ifdef DBGLOG
    std::cout << "DROP SQL: " << sql << std::endl;
#endif

    char * error = nullptr;
    int sql_result =  sqlite3_exec(mp_sqlite, sql.c_str(),
        [](void * , int, char ** , char **) {
            return 0;
        }, nullptr, &error);
    if(SQLITE_OK != sql_result)
    {
        std::string er_string("Unable to delete e-mails from the database:\n");
        er_string += error;
        sqlite3_free(error);
        throw StorageException(formatSqliteError(er_string, sql_result));
    }
    for(const std::unique_ptr<Email> & email : *emails)
    {
        OS::deleteFile(email->dataFilePath());
    }
    return emails->size();
}

void Repository::mapEdslToSqlSelectWhere(const Edsl::Expression & _expression, std::ostream & _out)
{
    if(!_expression.conditions.is_initialized())
    {
        return;
    }
    _out << " WHERE ";
    EdsToSqlMapper mapper(_out);
    mapper.mapToSqlWhereClause(*_expression.conditions);
}
