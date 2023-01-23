set(Server_SOURCES
    server.hpp
    json_server.hpp
    # Add any additional server source files here
)

add_library(Server ${Server_SOURCES})
target_compile_options(Server PRIVATE -std=c++20)
