cmake_minimum_required(VERSION 3.22)
project(malicious_PPML)

set(CMAKE_CXX_STANDARD 17)

add_executable(malicious_PPML
        main.cpp src/share/Share.cpp src/share/Spdz2kShare.cpp src/protocols/OperationOffline.cpp src/protocols/OperationOffline.h)
target_include_directories(malicious_PPML PUBLIC src)

add_subdirectory(src/offline)
