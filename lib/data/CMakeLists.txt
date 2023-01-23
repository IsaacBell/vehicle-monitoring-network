set(Data_SOURCES
    obd2_dictionary.hpp
    # obd2_translator.hpp
    # Add any additional data source files here
)

add_library(Data ${Data_SOURCES})
target_compile_options(Data PRIVATE -std=c++20)
