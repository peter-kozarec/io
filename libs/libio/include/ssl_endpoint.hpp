#ifndef IO_PRIV_SSL_ENDPOINT_HPP
#define IO_PRIV_SSL_ENDPOINT_HPP

#include "endpoint.hpp"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <memory>
#include <string>


namespace io::priv
{
    class SslEndpoint final : public Endpoint
    {
        using SocketType =
                boost::asio::ssl::stream<boost::asio::ip::tcp::socket>;

        std::unique_ptr<SocketType> socket_;

    public:
        SslEndpoint() = default;
        SslEndpoint(const SslEndpoint & other) = delete;
        SslEndpoint(SslEndpoint && other) noexcept = delete;
        SslEndpoint & operator=(const SslEndpoint & other) = delete;
        SslEndpoint & operator=(SslEndpoint && other) noexcept = delete;
        ~SslEndpoint() = default;

        boost::asio::awaitable<bool> connect() noexcept override;
        boost::asio::awaitable<bool> disconnect() noexcept override;

        boost::asio::awaitable<void> write(const std::string & message) override;
        boost::asio::awaitable<std::string> read() override;
    };
}

#endif // IO_PRIV_SSL_ENDPOINT_HPP
