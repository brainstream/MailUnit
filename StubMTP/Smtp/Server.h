#ifndef __STUBMTP_SMTP_PSERVER_H__
#define __STUBMTP_SMTP_PSERVER_H__

#include <cstdint>
#include <memory>
#include <atomic>
#include <boost/asio.hpp>
#include <StubMTP/Aux.h>
#include <StubMTP/Smtp/SessionProvider.h>

namespace StubMTP {
namespace Smtp {

class Server final
{
    STUBMTP_DISABLE_COPY(Server)
    struct ServerHolder;

public:
    static void startNew(boost::asio::io_service & _io_service, uint16_t _port, std::shared_ptr<SessionProvider> _provider);
    ~Server() = default;

private:
    Server(boost::asio::io_service & _io_service, uint16_t _port, std::shared_ptr<SessionProvider> _provider);
    void accept();
    void shutdown();

private:
    std::atomic<ServerHolder *> m_holder;
    std::atomic_bool m_shutdown;
    std::shared_ptr<SessionProvider> m_provider_ptr;
    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::asio::ip::tcp::socket m_socket;
}; // class Server

} // namespace Smtp
} // namespace StubMTP

#endif // __STUBMTP_SMTP_SERVER_H__
