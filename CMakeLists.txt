cmake_minimum_required(VERSION 3.15)
project(libio VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(libs/libio)

if(BUILD_EXAMPLES)
    add_subdirectory(examples/websocket_example)
endif()