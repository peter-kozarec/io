#ifndef IO_WEBSOCKET_HPP
#define IO_WEBSOCKET_HPP

#include "websocket_observer.hpp"
#include <boost/asio.hpp>
#include <memory>
#include <string>


namespace io
{
    class Websocket final
    {
        struct WebsocketImpl;
        WebsocketImpl * impl_;

    public:
        Websocket() = delete;
        explicit Websocket(std::weak_ptr<WebsocketObserver> observer);
        Websocket(const Websocket & other) = delete;
        Websocket(Websocket && other) noexcept = delete;
        Websocket & operator=(const Websocket & other) = delete;
        Websocket & operator=(Websocket && other) noexcept = delete;
        ~Websocket();

        void set_host(const std::string & host);
        std::string host() const;

        void set_port(const std::string & port);
        std::string port() const;

        void set_target(const std::string & target);
        std::string target() const;

        boost::asio::awaitable<bool> connect() noexcept;
        boost::asio::awaitable<bool> disconnect() noexcept;

        boost::asio::awaitable<void> send(const std::string & message);
        boost::asio::awaitable<void> listen();
    };
}

#endif // IO_WEBSOCKET_HPP