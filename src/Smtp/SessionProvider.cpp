#include <iostream>
#include <string>
#include "SessionProvider.h"

using namespace StubMTP::Smtp;

namespace {

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(boost::asio::ip::tcp::socket _socket, std::shared_ptr<SessionProvider> _provider);
    void start();

private:
    void write(const std::string & _message);
    void read();

private:
    boost::asio::ip::tcp::socket m_socket;
    std::shared_ptr<SessionProvider> m_provider_ptr;
    boost::asio::streambuf m_buffer; // TODO: move to the using point!
}; // class Session

} // namespace

SessionProvider::SessionProvider(boost::asio::io_service & _io_service) :
    mr_io_service(_io_service)
{
}

SessionProvider::~SessionProvider()
{
}

void SessionProvider::startNewSession(boost::asio::ip::tcp::socket _socket)
{
    std::make_shared<Session>(std::move(_socket), shared_from_this())->start();
}

Session::Session(boost::asio::ip::tcp::socket _socket, std::shared_ptr<SessionProvider> _provider) :
    m_socket(std::move(_socket)),
    m_provider_ptr(_provider)
{
}

void Session::start()
{
    write("220 [127.0.0.1] Stub Mail Transfer Service Ready\n");
}

void Session::write(const std::string & _message)
{
    auto self(shared_from_this());
    m_socket.async_send(boost::asio::buffer(_message),
        [this, self](const boost::system::error_code & ec, std::size_t length)
        {
            read();
        });
}

void Session::read()
{
    static std::string until("\r\n");
    m_buffer.prepare(1024);

    auto self(shared_from_this());
    boost::asio::async_read_until(m_socket, m_buffer, until,//'\n',
        [this, self](const boost::system::error_code & ec, std::size_t length)
        {
            if(ec) return;
            std::cout << "length: " << length << std::endl;
            std::cout << "size: " << m_buffer.size() << std::endl;
            m_buffer.commit(length);
            std::istream is(&m_buffer);
//            std::string str;
//            std::getline(is, str);


            size_t size = m_buffer.size();
            char * str_data = new char[size + 1];
            memset(str_data, 0, size + 1);
            is.readsome(str_data, size);
            std::string str(str_data);
            delete [] str_data;

            std::cout << "data: " << str << std::endl;
            std::cout.flush();
            m_buffer.consume(m_buffer.size());
            if(::strncmp(str.c_str(), "DATA", 4) == 0)
            {
                until = "\r\n.\r\n";
                write("354 Intermediate\n");
                return;
            } else if(until.size() > 2)
            {
                until = "\r\n";
            }
            write("250 OK\n");
        });
}
