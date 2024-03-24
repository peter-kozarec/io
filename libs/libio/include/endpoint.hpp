#ifndef IO_PRIV_ENDPOINT_HPP
#define IO_PRIV_ENDPOINT_HPP

#include <boost/asio.hpp>
#include <memory>
#include <string>


namespace io::priv
{
    class Endpoint
    {
        using ResolveResult =
                boost::asio::ip::tcp::resolver::results_type;

        std::string host_;
        std::string port_;

    public:
        Endpoint() = default;
        Endpoint(const Endpoint & other) = delete;
        Endpoint(Endpoint && other) noexcept = delete;
        Endpoint & operator=(const Endpoint & other) = delete;
        Endpoint & operator=(Endpoint && other) noexcept = delete;
        ~Endpoint() = default;

        virtual boost::asio::awaitable<bool> connect() noexcept = 0;
        virtual boost::asio::awaitable<bool> disconnect() noexcept = 0;

        virtual boost::asio::awaitable<void> write(const std::string & message) = 0;
        virtual boost::asio::awaitable<std::string> read() = 0;

        void set_host(const std::string & host);
        std::string host() const;

        void set_port(const std::string & port);
        std::string port() const;

    protected:
        static boost::asio::awaitable<ResolveResult> resolve(std::string_view host, std::string_view service);
    };
}

#endif  // IO_PRIV_ENDPOINT_HPP
