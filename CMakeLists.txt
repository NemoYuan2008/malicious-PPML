cmake_minimum_required(VERSION 3.25)
project(malicious_PPML)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(malicious_PPML
        main.cpp src/share/Share.cpp src/share/Spdz2kShare.cpp)
add_executable(offline_fake src/offline/offline_fake_main.cpp)

target_link_libraries(malicious_PPML )
