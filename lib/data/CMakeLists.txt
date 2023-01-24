cmake_minimum_required(VERSION 3.16)

add_library(Data
    cassandra.hpp
    diagnostic.hpp
    fleet.hpp
    obd2_dictionary.hpp
    quadtree.hpp
    user.hpp
    vehicle.hpp
)

target_compile_options(Data PRIVATE -std=c++20)

