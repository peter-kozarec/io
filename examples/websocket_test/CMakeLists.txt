cmake_minimum_required(VERSION 3.15)
project(websocket_test)

set(CMAKE_CXX_STANDARD 20)

find_package(libio REQUIRED)
find_package(Boost COMPONENTS system log REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(websocket_test src/main.cpp)

target_link_libraries(websocket_test PRIVATE ${LIBIO_LIBRARIES})
target_compile_options(websocket_test PRIVATE "-fcoroutines")
