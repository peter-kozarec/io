#ifndef IO_WEBSOCKET_OBSERVER_HPP
#define IO_WEBSOCKET_OBSERVER_HPP

#include <boost/asio.hpp>
#include <string>


namespace io
{
    class WebsocketObserver
    {
    public:
        virtual ~WebsocketObserver() = default;

        virtual boost::asio::awaitable<void> on_recieved(const std::string & message) = 0;
    };
}

#endif // IO_WEBSOCKET_OBSERVER_HPP