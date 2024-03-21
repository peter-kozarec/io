#include "io/websocket.hpp"
#include "websocket_endpoint.hpp"
#include <boost/log/core.hpp>


struct io::Websocket::WebsocketImpl
{
    io::priv::WebsocketEndpoint endpoint;
    std::weak_ptr<WebsocketObserver> observer;
};

io::Websocket::Websocket(std::weak_ptr<WebsocketObserver> observer) :
        impl_(new WebsocketImpl)
{
    impl_->observer = std::move(observer);
}

io::Websocket::~Websocket()
{
    delete impl_;
}

void io::Websocket::set_host(const std::string & host)
{
    impl_->endpoint.set_host(host);
}

std::string io::Websocket::host() const
{
    return impl_->endpoint.host();
}

void io::Websocket::set_port(const std::string & port)
{
    impl_->endpoint.set_port(port);
}

std::string io::Websocket::port() const
{
    return impl_->endpoint.port();
}

void io::Websocket::set_target(const std::string & target)
{
    impl_->endpoint.set_target(target);
}

std::string io::Websocket::target() const
{
    return impl_->endpoint.target();
}

boost::asio::awaitable<bool> io::Websocket::connect() noexcept
{
    co_return co_await impl_->endpoint.connect();
}

boost::asio::awaitable<bool> io::Websocket::disconnect() noexcept
{
    co_return co_await impl_->endpoint.disconnect();
}

boost::asio::awaitable<void> io::Websocket::send(std::string_view message)
{
    co_await impl_->endpoint.write(message);
}

boost::asio::awaitable<void> io::Websocket::listen()
{
    for (;;)
    {
        const auto result = co_await impl_->endpoint.read();

        if (const auto observer = impl_->observer.lock())
        {
            co_await observer->on_recieved(result);
        }
    }
}
