cmake_minimum_required(VERSION 3.10)
project(simple_CLI)

set(CMAKE_CXX_STANDARD 20)

add_library(simple_CLI_lib STATIC
        Simple_CLI.cpp
        Simple_CLI.h
)

add_executable(test Test.cpp)

target_link_libraries(test simple_CLI_lib)
