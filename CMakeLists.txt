cmake_minimum_required(VERSION 3.10)
project(simple_CLI)

# Imposta lo standard C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Libreria header-only: chi la consuma aggiunge solo la directory di include
add_library(simple_CLI INTERFACE
        src/include.hpp)
target_include_directories(simple_CLI INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(simple_CLI INTERFACE cxx_std_20)

# Eseguibile di prova (l'eseguibile non viene installato/packaggiato)
add_executable(simple_CLI_test Test.cpp)
target_link_libraries(simple_CLI_test PRIVATE simple_CLI)

# Bridge dell'API C (c_api.cpp): chi consuma da C puro linka questa libreria
# (o aggiunge c_api.cpp direttamente alla propria build)
option(SIMPLE_CLI_BUILD_C_API "Compila il bridge dell'API C" OFF)
if(SIMPLE_CLI_BUILD_C_API)
    add_library(simple_cli_c_api STATIC src/c_api.cpp)
    target_link_libraries(simple_cli_c_api PUBLIC simple_CLI)
    target_compile_features(simple_cli_c_api PUBLIC cxx_std_20)
endif()
