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
#include <MailUnit/IO/AsyncLambdaWriter.h>
#include <MailUnit/IO/AsyncFileWriter.h>
#include <MailUnit/IO/AsyncSequenceOperation.h>
#include <MailUnit/Mqp/ServerRequestHandler.h>

#define MQP_ENDLINE "\r\n"

using namespace MailUnit::Mqp;
using namespace MailUnit::Storage;
using namespace MailUnit::Server;
using namespace MailUnit::IO;

typedef typename boost::asio::ip::tcp::socket TcpSocket;

namespace {

class Session final : public std::enable_shared_from_this<Session>, private boost::noncopyable
{
public:
    inline explicit Session(TcpSocket _socket, std::shared_ptr<Repository> _repository);
    ~Session();
    inline void start();

private:
    size_t findEndOfQuery(const char * _query_piece, size_t _length);
    void read();
    void processQuery();
    void writeEmails(std::shared_ptr<std::vector<std::unique_ptr<Email> > > _emails);
    void write(const std::string & _data, std::function<void()> _callback);

private:
    TcpSocket m_socket;
    std::shared_ptr<Repository> m_repository_ptr;
    static const size_t s_buffer_size = 256;
    char * mp_buffer;
    bool m_position_in_quoted_text;
    std::string m_query;
}; // class Session

} // namespace

void ServerRequestHandler::handleConnection(TcpSocket _socket)
{
    LOG_INFO << "New connection accepted by the storage server";
    std::make_shared<Session>(std::move(_socket), m_repository_ptr)->start();
}

bool ServerRequestHandler::handleError(const boost::system::error_code & _err_code)
{
    LOG_ERROR << "The storage server has stopped due an error: " << _err_code.message();
    return false;
}

Session::Session(TcpSocket _socket, std::shared_ptr<Repository> _repository) :
    m_socket(std::move(_socket)),
    m_repository_ptr(_repository),
    mp_buffer(new char[s_buffer_size]),
    m_position_in_quoted_text(false)
{
}

Session::~Session()
{
    delete [] mp_buffer;
}

void Session::start()
{
    read();
}

void Session::read()
{
    std::shared_ptr<Session> self(shared_from_this());
    m_socket.async_receive(boost::asio::buffer(mp_buffer, s_buffer_size),
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

void Session::processQuery()
{
    try
    {
        std::vector<std::unique_ptr<Email>> * emails = new std::vector<std::unique_ptr<Email>>();
        m_repository_ptr->findEmails(std::move(m_query), *emails);
        writeEmails(std::shared_ptr<std::vector<std::unique_ptr<Email>>>(emails));
    }
    catch(const StorageException & error) // TODO: EdslException
    {
        std::shared_ptr<Session> self(shared_from_this());
        std::stringstream message;
        message << "ERROR: " << error.what() << "\r\n";
        write(message.str(), [self]() {
            self->read();
        });
    }
}

void Session::writeEmails(std::shared_ptr<std::vector<std::unique_ptr<Email>>> _emails)
{
    using EmailSequenceOperation = AsyncSequenceOperation<
        std::vector<std::unique_ptr<Email>>, TcpSocket>;
    using EmailOperation = AsyncSequenceItemOperation<std::unique_ptr<Email>, TcpSocket>;

    auto self = this->shared_from_this();
    size_t total_count = _emails->size();
    std::shared_ptr<EmailSequenceOperation> emails_operation = EmailSequenceOperation::create(_emails,
        [self, total_count](EmailOperation & email_operation) {
            const std::unique_ptr<Email> & email = email_operation.item();
            std::shared_ptr<std::ifstream> file(new std::ifstream(email->dataFilePath().string()));
            if(!file->is_open())
            {
                // TODO: error
            }
            email_operation.addStep(std::make_unique<AsyncLambdaWriter<TcpSocket>>(
                [&email_operation, &email, total_count](std::ostream & stream) {
                    stream <<
                        "ITEM: " << email_operation.itemIndex() + 1 << '/' << total_count << MQP_ENDLINE <<
                        "SIZE: " << boost::filesystem::file_size(email->dataFilePath()) << MQP_ENDLINE <<
                        "ID: " << email->id() << MQP_ENDLINE <<
                        "SUBJECT: " << email->subject() << MQP_ENDLINE;
                    for(const std::string & address : email->addresses(Email::AddressType::from))
                        stream << "FROM: " << address << MQP_ENDLINE;
                    for(const std::string & address : email->addresses(Email::AddressType::to))
                        stream << "TO: " << address << MQP_ENDLINE;
                    for(const std::string & address : email->addresses(Email::AddressType::cc))
                        stream << "CC: " << address << MQP_ENDLINE;
                    for(const std::string & address : email->addresses(Email::AddressType::bcc))
                        stream << "BCC: " << address << MQP_ENDLINE;
                    stream << MQP_ENDLINE;
                }
            ));
            email_operation.addStep(std::make_unique<AsyncFileWriter<TcpSocket>>(file));
        }
    );
    emails_operation->run(m_socket, [](const boost::system::error_code &) {
        // TODO: error
        return true;
    });
}

void Session::write(const std::string & _data, std::function<void()> _callback)
{
    boost::asio::async_write(m_socket, boost::asio::buffer(_data),
        [_callback](const boost::system::error_code & ec, std::size_t length)
        {
            // TODO: error handling
            _callback();
        });
}

size_t Session::findEndOfQuery(const char * _query_piece, size_t _length)
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
