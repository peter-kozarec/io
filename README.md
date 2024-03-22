# libio: C++ Library for Asynchronous I/O with Boost.Asio and C++20 Coroutines

libio is a C++ library that provides asynchronous I/O functionality using Boost.Asio and C++20 coroutines. It simplifies the development of asynchronous I/O operations by leveraging the power of coroutines while benefiting from the robustness and performance of Boost.Asio.

## Features

- Asynchronous I/O operations with Boost.Asio.
- Utilizes C++20 coroutines for writing asynchronous code in a synchronous style.
- Easy-to-use interface for handling asynchronous tasks.

## Prerequisites

Before building and using the io library, make sure you have the following dependencies installed:

- CMake (version 3.16 or higher)
- Boost (version 1.75.0 or higher) with Boost.Asio support
- A C++20-compliant compiler (such as GCC 10 or Clang 11)

## Building

Follow these steps to build the libio library:

1. Clone the repository:

   ```bash
   git clone https://github.com/peter-kozarec/libio.git
   ```

2. Create a build directory and navigate into it:

   ```bash
   cd libio
   mkdir build
   cd build
   ```

3. Generate build files with CMake:

   ```bash
   cmake ..
   ```

4. Build the library:

   ```bash
   cmake --build .
   ```

## Usage

To use the libio library in your C++ project, using cmake:

   ```cmake
   cmake_minimum_required(VERSION 3.15)
   project(libio_test)

   set(CMAKE_CXX_STANDARD 20)
   
   find_package(libio REQUIRED)
   find_package(Boost COMPONENTS system log REQUIRED)
   find_package(OpenSSL REQUIRED)
   
   add_executable(libio_test main.cpp)
   
   target_link_libraries(libio_test PRIVATE ${LIBIO_LIBRARIES})
   target_compile_options(libio_test PRIVATE "-fcoroutines")
   ```

3. Start using libio's asynchronous I/O functionality in your code.

## Example

Here's a simple example demonstrating how to perform asynchronous I/O with libio:

```cpp
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
    websocket.set_target(".ws");

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

```

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---
