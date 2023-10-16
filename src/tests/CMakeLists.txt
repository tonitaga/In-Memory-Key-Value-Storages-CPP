cmake_minimum_required(VERSION 3.5...3.16)
project(Transactions_CPP_TEST)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-std=c++17 -O3 -Wall -Werror")

include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/../model/storages/map
        ${CMAKE_CURRENT_SOURCE_DIR}/../model/storages/unordered_map
)

add_executable(Transactions_CPP_TEST
        map_test.cc
        unordered_test_map.cc
)

target_link_libraries(Transactions_CPP_TEST gtest_main)
add_test(NAME Transactions_CPP_TEST_ COMMAND Transactions_CPP_TEST)
