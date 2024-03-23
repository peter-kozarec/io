#include "io/requests.hpp"
#include "ssl_endpoint.hpp"


struct io::Requests::RequestsImpl
{
    io::priv::SslEndpoint endpoint;
};


io::Requests::Requests() :
        impl_(new RequestsImpl)
{
}

io::Requests::~Requests()
{
    delete impl_;
}

void io::Requests::set_host(const std::string & host)
{
    impl_->endpoint.set_host(host);
}

std::string io::Requests::host() const
{
    return impl_->endpoint.host();
}

void io::Requests::set_port(const std::string & port)
{
    impl_->endpoint.set_port(port);
}

std::string io::Requests::port() const
{
    return impl_->endpoint.port();
}

boost::asio::awaitable<bool> io::Requests::connect() noexcept
{
    co_return co_await impl_->endpoint.connect();
}

boost::asio::awaitable<bool> io::Requests::disconnect() noexcept
{
    co_return co_await impl_->endpoint.disconnect();
}

boost::asio::awaitable<std::string> io::Requests::send(const std::string & request)
{
    co_await impl_->endpoint.write(request);
    co_return co_await impl_->endpoint.read();
}