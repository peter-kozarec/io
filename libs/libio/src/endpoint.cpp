#include "endpoint.hpp"


void io::priv::Endpoint::set_host(const std::string & host)
{
    host_ = host;
}

std::string io::priv::Endpoint::host() const
{
    return host_;
}

void io::priv::Endpoint::set_port(const std::string & port)
{
    port_ = port;
}

std::string io::priv::Endpoint::port() const
{
    return port_;
}

boost::asio::awaitable<io::priv::Endpoint::ResolveResult>
io::priv::Endpoint::resolve(std::string_view host, std::string_view service)
{
    boost::asio::ip::tcp::resolver resolver(co_await boost::asio::this_coro::executor);
    co_return co_await resolver.async_resolve(host, service, boost::asio::use_awaitable);
}