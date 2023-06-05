cmake_minimum_required(VERSION 3.25)
project(demo)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS thread)
include_directories(${Boost_INCLUDE_DIR})

add_library(network Network.cpp Network.h)

add_executable(player0 application/run-player0.cpp)
target_link_libraries(player0 network)
add_executable(player1 application/run-player1.cpp)
target_link_libraries(player1 network)
