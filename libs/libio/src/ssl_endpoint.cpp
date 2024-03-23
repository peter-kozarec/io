#include "ssl_endpoint.hpp"
#include <boost/log/trivial.hpp>


boost::asio::awaitable<bool> io::priv::SslEndpoint::connect() noexcept
{
    if (socket_ && socket_->lowest_layer().is_open())
    {
        BOOST_LOG_TRIVIAL(warning) << "ssl://" << host() << " already connected.";
        co_return true;
    }

    try
    {
        const auto executor = co_await boost::asio::this_coro::executor;

        boost::asio::ssl::context ssl_context(boost::asio::ssl::context::sslv23_client);
        socket_ = std::make_unique<SocketType>(executor, ssl_context);

        const auto endpoints = co_await resolve(host(), port());
        co_await boost::asio::async_connect(socket_->next_layer(), endpoints, boost::asio::use_awaitable);

        const auto result = SSL_set_tlsext_host_name(socket_->native_handle(), host().c_str());

        if ( result != 1)
        {
            BOOST_LOG_TRIVIAL(warning) << "SSL_set_tlsext_host_name failed with result: " << result;
            co_return false;
        }

        co_await socket_->async_handshake(boost::asio::ssl::stream_base::client, boost::asio::use_awaitable);

        socket_->set_verify_mode(boost::asio::ssl::verify_peer);

        BOOST_LOG_TRIVIAL(debug) << "ssl://" << host() << " connected";
        co_return true;
    }
    catch (const std::exception & e)
    {
        BOOST_LOG_TRIVIAL(warning) << "SslEndpoint::connect " << e.what();
        co_return false;
    }
}

boost::asio::awaitable<bool> io::priv::SslEndpoint::disconnect() noexcept
{
    if (socket_ && socket_->lowest_layer().is_open())
    {
        try
        {
            co_await socket_->async_shutdown(boost::asio::use_awaitable);
            BOOST_LOG_TRIVIAL(debug) << "ssl://" << host() << " disconnected";
        }
        catch (const std::exception & e)
        {
            BOOST_LOG_TRIVIAL(warning) << "SslEndpoint::disconnect " << e.what();
            co_return false;
        }
    }

    co_return true;
}

boost::asio::awaitable<void> io::priv::SslEndpoint::write(const std::string & message)
{
    if (!socket_ || !socket_->lowest_layer().is_open())
    {
        throw std::runtime_error("Ssl socket to ssl://" + host() + " is not opened.");
    }

    BOOST_LOG_TRIVIAL(trace) << "ssl://" << host() << " << " << message;

    boost::asio::const_buffer buffer(message.data(), message.size());
    co_await boost::asio::async_write(*socket_, buffer, boost::asio::use_awaitable);
}

boost::asio::awaitable<std::string> io::priv::SslEndpoint::read()
{
    if (!socket_ || !socket_->lowest_layer().is_open())
    {
        throw std::runtime_error("Ssl socket to ssl://" + host() + " is not opened.");
    }

    boost::asio::streambuf buffer;
    co_await boost::asio::async_read(*socket_, buffer, boost::asio::use_awaitable);
    const auto response = boost::beast::buffers_to_string(buffer.data());
    BOOST_LOG_TRIVIAL(trace) << "ssl://" << host() << " >> " << response;
    co_return response;
}