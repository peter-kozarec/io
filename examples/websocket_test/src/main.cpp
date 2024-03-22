#include "io/websocket.hpp"
#include <iostream>


class Observer final : public io::WebsocketObserver
{
public:
    ~Observer() override = default;

    boost::asio::awaitable<void> on_recieved(const std::string & message)
    {
        std::cout << message << std::endl;
        co_return;
    }
};

int main(int argc, char ** argv)
{
    const auto observer = std::make_shared<Observer>();
    io::Websocket websocket(observer);

    websocket.set_host("echo.websocket.org");
    websocket.set_port("443");
    websocket.set_target("/.ws");

    boost::asio::io_context context;

    boost::asio::co_spawn(context, [&websocket]() -> boost::asio::awaitable<void>
        {
            co_await websocket.connect();
            co_await websocket.send("Hello World");
            co_await websocket.listen();
        }, boost::asio::detached);

    context.run();

    return EXIT_SUCCESS;
}
