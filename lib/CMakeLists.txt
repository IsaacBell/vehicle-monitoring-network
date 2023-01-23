cmake_minimum_required(VERSION 3.10)

project(OBD2-Reader)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED COMPONENTS system program_options geometry)

set(Data_SOURCES
    data/obd2_dictionary.hpp
    # data/obd2_translator.hpp
    # Add any additional data source files here
)
set(Server_SOURCES
    server/server.hpp
    server/json_server.hpp
    # Add any additional server source files here
)

add_subdirectory(data)
add_subdirectory(server)

# add_library(Data ${Data_SOURCES})
# target_compile_options(Data PRIVATE -std=c++20)

# add_library(Server ${Server_SOURCES})
# target_compile_options(Server PRIVATE -std=c++20)

add_executable(OBD2-Reader main.cpp)
target_compile_options(OBD2-Reader PRIVATE -std=c++20)

target_link_libraries(OBD2-Reader PRIVATE Data Server Boost::system Boost::geometry Boost::program_options)

# add_dependencies(OBD2-Reader Data Server)
