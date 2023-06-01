cmake_minimum_required(VERSION 3.25)
project(demo)

set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_CXX_FLAGS "-march=haswell -mavx512f")

#set(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wextra -O3")

add_library(NETWORK network.cpp network.h)
target_link_libraries(NETWORK PUBLIC pthread)

add_executable(player0 application/run-player0.cpp)
target_link_libraries(player0 NETWORK)
add_executable(player1 application/run-player1.cpp)
target_link_libraries(player1 NETWORK)