cmake_minimum_required(VERSION 3.10)
project(simple_CLI)

# Imposta lo standard C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Aggiunge l'eseguibile (senza virgole tra i file)
add_executable(simple_CLI Simple_CLI.cpp Test.cpp)
