#include "websocket_endpoint.hpp"
#include <boost/log/trivial.hpp>


boost::asio::awaitable<bool> io::priv::WebsocketEndpoint::connect() noexcept
{
    if (socket_ && socket_->is_open())
    {
        BOOST_LOG_TRIVIAL(warning) << "ws://" << host() << target() << " already connected.";
        co_return true;
    }

    try
    {
        boost::asio::ip::tcp::socket tcp_socket(co_await boost::asio::this_coro::executor);

        const auto endpoints = co_await resolve(host(), port());
        co_await boost::asio::async_connect(tcp_socket, endpoints, boost::asio::use_awaitable);

        boost::asio::ssl::context ssl_context(boost::asio::ssl::context::sslv23_client);
        boost::beast::ssl_stream<boost::asio::ip::tcp::socket> ssl_stream(std::move(tcp_socket), ssl_context);

        SSL_set_tlsext_host_name(ssl_stream.native_handle(), host().c_str());

        co_await ssl_stream.async_handshake(boost::asio::ssl::stream_base::client, boost::asio::use_awaitable);

        socket_ = std::make_unique<SocketType>(std::move(ssl_stream));
        socket_->handshake(host(), target_);

        BOOST_LOG_TRIVIAL(debug) << "ws://" << host() << target() << " connected";
        co_return true;
    }
    catch (const std::exception & e)
    {
        BOOST_LOG_TRIVIAL(warning) << "WsEndpoint::connect" << e.what();
        co_return false;
    }
}

boost::asio::awaitable<bool> io::priv::WebsocketEndpoint::disconnect() noexcept
{
    if (socket_ && socket_->is_open())
    {
        try
        {
            co_await socket_->async_close(boost::beast::websocket::close_code::normal, boost::asio::use_awaitable);
            BOOST_LOG_TRIVIAL(debug) << "ws://" << host() << target() << " disconnected";
        }
        catch (const std::exception & e)
        {
            BOOST_LOG_TRIVIAL(warning) << "WsEndpoint::disconnect " << e.what();
            co_return false;
        }
    }

    co_return true;
}

boost::asio::awaitable<void> io::priv::WebsocketEndpoint::write(const std::string & message)
{
    if (!socket_ || !socket_->is_open())
    {
        throw std::runtime_error("Websocket to ws://" + host() + target() + " is not opened.");
    }

    BOOST_LOG_TRIVIAL(trace) << "ws://" << host() << target() << " << " << message;

    co_await socket_->async_write(boost::asio::buffer(message), boost::asio::use_awaitable);
}

boost::asio::awaitable<std::string> io::priv::WebsocketEndpoint::read()
{
    if (!socket_ || !socket_->is_open())
    {
        throw std::runtime_error("Websocket to ws://" + host() + target() + " is not opened.");
    }

    boost::beast::flat_buffer buffer;
    co_await socket_->async_read(buffer, boost::asio::use_awaitable);
    const auto response = boost::beast::buffers_to_string(buffer.data());
    BOOST_LOG_TRIVIAL(trace) << "ws://" << host() << target() << " >> " << response;
    co_return response;
}

void io::priv::WebsocketEndpoint::set_target(const std::string & target)
{
    target_ = target;
}

std::string io::priv::WebsocketEndpoint::target() const
{
    return target_;
}