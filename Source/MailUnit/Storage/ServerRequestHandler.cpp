#include <memory>
#include <MailUnit/Storage/ServerRequestHandler.h>
#include <MailUnit/Application.h>

using namespace MailUnit::Storage;

namespace {

class Session final : public std::enable_shared_from_this<Session>
{
    MU_DISABLE_COPY(Session)

public:
    inline explicit Session(boost::asio::ip::tcp::socket _socket);
    ~Session();
    inline void start();

private:
    size_t findEndOfQuery(const char * _query_piece, size_t _length);
    void read();
    void processQuery();
    void write(const std::string & _text);

private:
    boost::asio::ip::tcp::socket m_socket;
    static const size_t s_buffer_size = 256;
    char * mp_buffer;
    bool m_position_in_quoted_text;
    std::string m_query;
}; // class Session

} // namespace

void ServerRequestHandler::handleConnection(boost::asio::ip::tcp::socket _socket)
{
    MailUnit::app().log().info("New connection accepted by the storage server");
    std::make_shared<Session>(std::move(_socket))->start();
}

bool ServerRequestHandler::handleError(const boost::system::error_code & _err_code)
{
    std::string message("The storage server has stopped due an error: ");
    MailUnit::app().log().error(message + _err_code.message());
    return false;
}

Session::Session(boost::asio::ip::tcp::socket _socket) :
    m_socket(std::move(_socket)),
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
            size_t end_pos = self->findEndOfQuery(self->mp_buffer, length);
            self->m_query.append(self->mp_buffer, &self->mp_buffer[length - 1]);
            if(end_pos != length)
                self->processQuery();
            else
                self->read();
        });
}

void Session::processQuery()
{
    // TODO: make query to a DB
    write("Test response!");
}

void Session::write(const std::string & _text)
{
    std::shared_ptr<Session> self(shared_from_this());
    m_socket.async_send(boost::asio::buffer(_text),
        [self](const boost::system::error_code & ec, std::size_t length)
        {
            self->read();
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
