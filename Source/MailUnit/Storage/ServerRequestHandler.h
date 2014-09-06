#ifndef __MU_STORAGE_SESSION_H__
#define __MU_STORAGE_SESSION_H__

#include <boost/asio.hpp>
#include <MailUnit/Server/RequestHandler.h>

namespace MailUnit {
namespace Storage {

class ServerRequestHandler : public Server::RequestHandler<boost::asio::ip::tcp::socket>
{
public:
    void handleConnection(boost::asio::ip::tcp::socket _socket) override;
    bool handleError(const boost::system::error_code & _err_code) override;
}; // class ServerRequestHandler

} // namespace Storage
} // namespace MailUnit

#endif // __MU_STORAGE_SESSION_H__
