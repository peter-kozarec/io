set(Boost_USE_STATIC_LIBS ON)

find_package(Boost COMPONENTS system log REQUIRED)
find_package(OpenSSL REQUIRED)

add_library(io)

target_sources(io
        PRIVATE
            src/requests.cpp
            src/websocket.cpp
            src/endpoint.cpp
            src/ssl_endpoint.cpp
            src/websocket_endpoint.cpp
)

target_include_directories(io
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        PRIVATE
            include
)

target_link_libraries(io PRIVATE ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES})
target_compile_options(io PRIVATE "-fcoroutines")
target_compile_definitions(io PRIVATE "BOOST_ALLOW_DEPRECATED_HEADERS" "BOOST_LOG_DYN_LINK=1")

# Export targets
install(TARGETS io
        EXPORT libioTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

# Headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/io
        DESTINATION include
)

# Target cmake file
install(EXPORT libioTargets
        FILE libioTargets.cmake
        DESTINATION lib/cmake/libio
)

# Config cmake file
include(CMakePackageConfigHelpers)
configure_package_config_file(
        "${CMAKE_SOURCE_DIR}/Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/libioConfig.cmake"
        INSTALL_DESTINATION "lib/cmake/libio"
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libioConfig.cmake"
        DESTINATION "lib/cmake/libio"
)

# Config Version cmake file
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/libioConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libioConfigVersion.cmake"
        DESTINATION "lib/cmake/libio"
)