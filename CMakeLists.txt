cmake_minimum_required(VERSION 3.22)
project(malicious_PPML)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(malicious_PPML
        main.cpp src/share/Share.cpp src/protocols/OperationOffline.cpp src/protocols/OperationOffline.h)
add_library(uint128 src/share/uint128.cpp src/share/uint128.h)

target_link_libraries(malicious_PPML uint128 Spdz2kShare)

add_subdirectory(src/offline)
add_subdirectory(src/share)

add_subdirectory(tests)
