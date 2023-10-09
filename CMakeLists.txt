cmake_minimum_required(VERSION 3.26)
project(Transactions_Library_CPP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall")

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/src/model/command
        ${CMAKE_CURRENT_SOURCE_DIR}/src/model/student
        ${CMAKE_CURRENT_SOURCE_DIR}/src/extern
)

add_executable(Transactions_Library_CPP
        src/main.cc)
