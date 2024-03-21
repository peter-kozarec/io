#ifndef IO_PRIV_WEBSOCKET_ENDPOINT_HPP
#define IO_PRIV_WEBSOCKET_ENDPOINT_HPP

#include "endpoint.hpp"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <memory>
#include <string>


namespace io::priv
{
    class WebsocketEndpoint final : public Endpoint
    {
        using SocketType =
                boost::beast::websocket::stream<boost::beast::ssl_stream<boost::asio::ip::tcp::socket>>;

        std::string target_;
        std::unique_ptr<SocketType> socket_;

    public:
        WebsocketEndpoint() = default;
        WebsocketEndpoint(const WebsocketEndpoint & other) = delete;
        WebsocketEndpoint(WebsocketEndpoint && other) noexcept = delete;
        WebsocketEndpoint & operator=(const WebsocketEndpoint & other) = delete;
        WebsocketEndpoint & operator=(WebsocketEndpoint && other) noexcept = delete;
        ~WebsocketEndpoint() = default;

        boost::asio::awaitable<bool> connect() noexcept override;
        boost::asio::awaitable<bool> disconnect() noexcept override;

        boost::asio::awaitable<void> write(std::string_view message) override;
        boost::asio::awaitable<std::string> read() override;

        void set_target(const std::string & target);
        std::string target() const;
    };
}

#endif // IO_PRIV_WEBSOCKET_ENDPOINT_HPP
