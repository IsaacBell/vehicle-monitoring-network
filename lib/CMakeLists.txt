cmake_minimum_required(VERSION 3.16)

project(OBD2-Reader)

set(CMAKE_CXX_STANDARD 20)

# todo - temp
include_directories("/opt/homebrew/Cellar/boost/1.81.0_1/include/")
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/boost/1.81.0_1")

set(BOOST_ROOT "/opt/homebrew/Cellar/boost/1.81.0_1")
set(BOOST_INCLUDEDIR "${BOOST_ROOT}/include/")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib")
set(Boost_NO_SYSTEM_PATHS TRUE)
find_package(Boost 1.81.0 COMPONENTS geometry REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

find_package(Boost COMPONENTS system program_options geometry REQUIRED)

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
