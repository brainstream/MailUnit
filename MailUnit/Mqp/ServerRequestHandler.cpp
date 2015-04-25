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

#include <memory>
#include <sstream>
#include <functional>
#include <boost/noncopyable.hpp>
#include <MailUnit/Logger.h>
#include <MailUnit/Server/Tcp/TcpSession.h>
#include <MailUnit/IO/AsyncLambdaWriter.h>
#include <MailUnit/IO/AsyncFileWriter.h>
#include <MailUnit/IO/AsyncSequenceOperation.h>
#include <MailUnit/Mqp/ServerRequestHandler.h>
#include <MailUnit/Mqp/Error.h>

#define MQP_ENDLINE   "\r\n"
#define MQP_ITEM      "ITEM: "
#define MQP_SIZE      "SIZE: "
#define MQP_ID        "ID: "
#define MQP_SUBJECT   "SUBJECT: "
#define MQP_FROM      "FROM: "
#define MQP_TO        "TO: "
#define MQP_CC        "CC: "
#define MQP_BCC       "BCC: "
#define MQP_ERROR     "ERROR: "
#define MQP_DELETED   "DELETED: "

using namespace MailUnit::Mqp;
using namespace MailUnit::Storage;
using namespace MailUnit::Server;
using namespace MailUnit::IO;

namespace {

class MqpSession final :
    public std::enable_shared_from_this<MqpSession>,
    public TcpSession
{
    typedef typename AsyncSequenceOperation<std::vector<std::unique_ptr<Email>>>::SequenceHolder EmailsHolder;

public:
    inline explicit MqpSession(TcpSocket _socket, std::shared_ptr<Repository> _repository);
    ~MqpSession();
    void start() override;

private:
    size_t findEndOfQuery(const char * _query_piece, size_t _length);
    void read();
    void processQuery();
    bool isQueryEndOfSessionRequest(const std::string & _query);
    void writeEmails(EmailsHolder _emails);
    void writeError(ErrorCode _code, const std::exception * _exception);
    void write(const std::string & _data, std::function<void()> _callback);

private:
    std::shared_ptr<Repository> m_repository_ptr;
    static const size_t s_buffer_size = 256;
    char * mp_buffer;
    bool m_position_in_quoted_text;
    std::string m_query;
}; // class MqpSession

} // namespace

std::shared_ptr<Session> ServerRequestHandler::createSession(TcpSocket _socket)
{
    return std::make_shared<MqpSession>(std::move(_socket), m_repository_ptr);
}

bool ServerRequestHandler::handleError(const boost::system::error_code & _err_code)
{
    LOG_FATAL << "The storage server has stopped due an error: " << _err_code.message();
    return false;
}

MqpSession::MqpSession(TcpSocket _socket, std::shared_ptr<Repository> _repository) :
    TcpSession(std::move(_socket)),
    m_repository_ptr(_repository),
    mp_buffer(new char[s_buffer_size]),
    m_position_in_quoted_text(false)
{
    LOG_DEBUG << "New MQP session has started";
}

MqpSession::~MqpSession()
{
    LOG_DEBUG << "MQP session has closed";
    delete [] mp_buffer;
}

void MqpSession::start()
{
    read();
}

void MqpSession::read()
{
    std::shared_ptr<MqpSession> self(shared_from_this());
    readAsync(boost::asio::buffer(mp_buffer, s_buffer_size),
        [self](const boost::system::error_code & ec, std::size_t length)
        {
            if(ec) return; // TODO: log
            self->mp_buffer[length] = '\0';
            size_t end_pos = self->findEndOfQuery(self->mp_buffer, length);
            if(end_pos == length)
            {
                self->m_query.append(self->mp_buffer, &self->mp_buffer[length - 1]);
                self->read();
            }
            else
            {
                self->m_query.append(self->mp_buffer, &self->mp_buffer[end_pos]);
                self->processQuery();
            }
        });
}

void MqpSession::processQuery()
{
    std::string query = boost::trim_copy(m_query);
    m_query.clear();
    if(isQueryEndOfSessionRequest(query))
    {
        return;
    }
    try
    {
        std::shared_ptr<QueryResult> query_result = m_repository_ptr->executeQuery(query);
        QueryGetResult * get = boost::get<QueryGetResult>(query_result.get());
        if(get)
        {
            writeEmails([query_result, get]() -> const std::vector<std::unique_ptr<Email>> & {
                return get->emails;
            });
            return;
        }
        QueryDropResult * drop = boost::get<QueryDropResult>(query_result.get());
        if(drop)
        {
            std::shared_ptr<MqpSession> self(shared_from_this());
            std::stringstream message;
            message << MQP_DELETED << drop->count << MQP_ENDLINE;
            write(message.str(), [self]() {
                self->read();
            });
            return;
        }
    }
    catch(const StorageException & error)
    {
        writeError(ErrorCode::StorageError, &error);
    }
    catch(const Edsl::EdslException & error)
    {
        writeError(ErrorCode::ParseError, &error);
    }
    catch(const std::exception & error)
    {
        writeError(ErrorCode::UnknowError, &error);
    }
    catch(...)
    {
        writeError(ErrorCode::UnknowError, nullptr);
    }
}

bool MqpSession::isQueryEndOfSessionRequest(const std::string & _query)
{
    return boost::algorithm::iequals("quit", _query) || boost::algorithm::iequals("q", _query);
}

void MqpSession::writeEmails(EmailsHolder _emails)
{
    using EmailSequenceOperation = AsyncSequenceOperation<std::vector<std::unique_ptr<Email>>>;
    using EmailOperation = AsyncSequenceItemOperation<std::unique_ptr<Email>>;

    auto self = this->shared_from_this();
    size_t total_count = _emails().size();
    if(0 == total_count)
    {
        std::string message = MQP_ITEM "0/0" MQP_ENDLINE MQP_SIZE "0" MQP_ENDLINE;
        write(message, [self]() {
            self->read();
        });
        return;
    }
    std::shared_ptr<EmailSequenceOperation> emails_operation = EmailSequenceOperation::create(_emails,
        [self, total_count](EmailOperation & email_operation) {
            const std::unique_ptr<Email> & email = email_operation.item();
            std::shared_ptr<std::ifstream> file(new std::ifstream(email->dataFilePath().string()));
            if(!file->is_open())
            {
                // TODO: error
            }
            email_operation.addStep(std::make_unique<AsyncLambdaWriter>(
                [&email_operation, &email, total_count](std::ostream & stream) {
                    stream <<
                        MQP_ITEM << email_operation.itemIndex() + 1 << '/' << total_count << MQP_ENDLINE <<
                        MQP_SIZE << boost::filesystem::file_size(email->dataFilePath()) << MQP_ENDLINE <<
                        MQP_ID << email->id() << MQP_ENDLINE <<
                        MQP_SUBJECT << email->subject() << MQP_ENDLINE;
                    for(const std::string & address : email->addresses(Email::AddressType::from))
                        stream << MQP_FROM << address << MQP_ENDLINE;
                    for(const std::string & address : email->addresses(Email::AddressType::to))
                        stream << MQP_TO << address << MQP_ENDLINE;
                    for(const std::string & address : email->addresses(Email::AddressType::cc))
                        stream << MQP_CC << address << MQP_ENDLINE;
                    for(const std::string & address : email->addresses(Email::AddressType::bcc))
                        stream << MQP_BCC << address << MQP_ENDLINE;
                    stream << MQP_ENDLINE;
                }
            ));
            email_operation.addStep(std::make_unique<AsyncFileWriter>(file));
        }
    );
    emails_operation->run(*this, [self](const boost::system::error_code &) {
        // TODO: error
        self->read();
        return true;
    });
}

void MqpSession::writeError(ErrorCode _code, const std::exception * _exception)
{
    std::shared_ptr<MqpSession> self(shared_from_this());
    std::stringstream message;
    message << MQP_ERROR << _code << MQP_ENDLINE;
    write(message.str(), [self]() {
        self->read();
    });
    if(nullptr == _exception)
        LOG_ERROR << "An unknonw error occurred during the MQP query processing";
    else
        LOG_ERROR << "An error occurred during the MQP query processing: " << _exception->what();
}

void MqpSession::write(const std::string & _data, std::function<void()> _callback)
{
    writeAsync(boost::asio::buffer(_data),
        [_callback](const boost::system::error_code & ec, std::size_t length)
        {
            // TODO: error handling
            _callback();
        });
}

size_t MqpSession::findEndOfQuery(const char * _query_piece, size_t _length)
{
    for(size_t i = 0; i < _length; ++i)
    {
        char symbol = _query_piece[i];
        if(m_position_in_quoted_text)
        {
            if('\'' == symbol)
                m_position_in_quoted_text = false;
            continue;
        }
        switch(symbol)
        {
        case '\'':
            m_position_in_quoted_text = true;
            break;
        case ';':
            return i;
        }
    }
    return _length;
}