SET(Boost_USE_STATIC_LIBS ON)

find_package(Boost COMPONENTS system log REQUIRED)
find_package(OpenSSL REQUIRED)

# Create target for the foundation library
add_library(io)

# Specifies sources to use when building a target
target_sources(io
        PRIVATE
            src/requests.cpp
            src/websocket.cpp
            src/endpoint.cpp
            src/ssl_endpoint.cpp
            src/websocket_endpoint.cpp
)

# Specifies include directories for given target
target_include_directories(io
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        PRIVATE
            include
)

# Specify libraries or flags for given target
target_link_libraries(io
        PRIVATE
            ${Boost_LIBRARIES}
            ${OPENSSL_LIBRARIES}
)

# Specify compile options
target_compile_options(io
        PRIVATE
            "-fcoroutines"
)

# Specify compile definitions
target_compile_definitions(io
        PRIVATE
            "BOOST_ALLOW_DEPRECATED_HEADERS"
            "BOOST_LOG_DYN_LINK=1"
)