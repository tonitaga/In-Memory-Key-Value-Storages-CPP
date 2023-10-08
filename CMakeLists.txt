cmake_minimum_required(VERSION 3.26)
project(Transactions_Library_CPP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Werror -Wextra")

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/src/in_memory_key_value/bplus_tree
        ${CMAKE_CURRENT_SOURCE_DIR}/src/in_memory_key_value/red_black_tree
        ${CMAKE_CURRENT_SOURCE_DIR}/src/in_memory_key_value/unordered_map
)

add_executable(Transactions_Library_CPP
        src/main.cc)
