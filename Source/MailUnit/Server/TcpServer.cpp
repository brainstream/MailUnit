#include <MailUnit/Server/TcpServer.h>

using namespace MailUnit::Server;
namespace asio = boost::asio;

namespace {

class TcpServer : public std::enable_shared_from_this<TcpServer>
{
public:
    inline TcpServer(asio::io_service & _io_service,
        const asio::ip::tcp::endpoint & _endpoint,
        std::shared_ptr<TcpRequestHandler> _handler);
    void accept();

private:
    asio::ip::tcp::socket m_socket;
    asio::ip::tcp::acceptor m_acceptor;
    std::shared_ptr<TcpRequestHandler> m_handler_ptr;
}; // class TcpServer

} // namespace

TcpServer::TcpServer(asio::io_service & _io_service,
        const asio::ip::tcp::endpoint & _endpoint,
        std::shared_ptr<TcpRequestHandler> _handler) :
    m_socket(_io_service),
    m_acceptor(_io_service, _endpoint),
    m_handler_ptr(_handler)
{
}

void TcpServer::accept()
{
    if(nullptr == m_handler_ptr)
    {
        return;
    }
    auto self = shared_from_this();
    m_acceptor.async_accept(m_socket, [self](boost::system::error_code err_code)
    {
        if(err_code)
        {
            if(!self->m_handler_ptr->handleError(err_code))
                return;
        }
        else
        {
            self->m_handler_ptr->handleConnection(std::move(self->m_socket));
        }
        self->accept();
    });
}

void MailUnit::Server::startTcpServer(asio::io_service & _io_service,
    const asio::ip::tcp::endpoint & _endpoint,
    std::shared_ptr<TcpRequestHandler> _handler)
{
    std::make_shared<TcpServer>(_io_service, _endpoint, _handler)->accept();
}
