cmake_minimum_required(VERSION 3.5...3.16)
project(Transactions_CPP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-std=c++17 -O3 -Wall -Werror")

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/src/model/command
        ${CMAKE_CURRENT_SOURCE_DIR}/src/model/student
        ${CMAKE_CURRENT_SOURCE_DIR}/src/extern
        ${CMAKE_CURRENT_SOURCE_DIR}/src/view
        ${CMAKE_CURRENT_SOURCE_DIR}/src/model/storages/map
        ${CMAKE_CURRENT_SOURCE_DIR}/src/model/storages/unordered_map
        ${CMAKE_CURRENT_SOURCE_DIR}/src/model/functions
)

add_executable(Transactions_CPP
        src/main.cc
        src/model/student/student.cc
        src/view/view.cc)