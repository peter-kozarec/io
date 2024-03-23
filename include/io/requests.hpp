#ifndef IO_REQUESTS_HPP
#define IO_REQUESTS_HPP

#include <boost/asio.hpp>
#include <string>


namespace io
{
    class Requests final
    {
        struct RequestsImpl;
        RequestsImpl * impl_;

    public:
        Requests();
        Requests(const Requests &) = delete;
        Requests(Requests &&) noexcept = delete;
        Requests & operator=(const Requests &) = delete;
        Requests & operator=(Requests &&) noexcept = delete;
        ~Requests();

        void set_host(const std::string & host);
        std::string host() const;

        void set_port(const std::string & port);
        std::string port() const;

        boost::asio::awaitable<bool> connect() noexcept;
        boost::asio::awaitable<bool> disconnect() noexcept;

        boost::asio::awaitable<std::string> send(const std::string & request);
    };
}

#endif // IO_REQUESTS_HPP
