#include <iostream>
#include <StubMTP/Smtp/Server.h>

using namespace StubMTP::Smtp;

struct Server::ServerHolder
{
    std::shared_ptr<Server> server;
};

void Server::startNew(boost::asio::io_service & _io_service, uint16_t _port, std::shared_ptr<SessionProvider> _provider)
{
    Server * server = new Server(_io_service, _port, _provider);
    std::shared_ptr<Server> ptr = std::shared_ptr<Server>(server);
    ServerHolder * holder = new ServerHolder();
    holder->server = ptr;
    server->m_holder = holder;
    server->accept();
}

Server::Server(boost::asio::io_service & _io_service, uint16_t _port, std::shared_ptr<SessionProvider> _provider) :
    m_holder(nullptr),
    m_shutdown(false),
    m_provider_ptr(_provider),
    m_acceptor(_io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), _port)),
    m_socket(_io_service)
{
}

void Server::accept()
{
    std::shared_ptr<SessionProvider> provider = m_provider_ptr;
    m_acceptor.async_accept(m_socket, [this, provider](boost::system::error_code err_code)
    {
        if(m_shutdown)
        {
            return;
        }
        if(err_code)
        {
            std::cerr << "Server error: " << err_code.message() << std::endl;
            shutdown();
        }
        else
        {
            provider->startNewSession(std::move(m_socket));
            accept();
        }
    });

}

void Server::shutdown()
{
    std::cout << "Shutting down...\n";
    if(m_shutdown.exchange(true))
    {
        std::cout << "Already shutdown\n";
        return;
    }
    delete m_holder.exchange(nullptr);
}
