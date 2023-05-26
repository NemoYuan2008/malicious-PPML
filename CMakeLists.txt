cmake_minimum_required(VERSION 3.22)
project(malicious_PPML)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(malicious_PPML
        main.cpp src/share/Share.cpp src/protocols/GateOffline.cpp src/protocols/GateOffline.h src/offline/FakeOffline.cpp src/protocols/Gate.cpp src/protocols/Gate.h src/utils/rand.h)

target_link_libraries(malicious_PPML uint128 Spdz2kShare)

add_subdirectory(src/offline)
add_subdirectory(src/share)

add_subdirectory(tests)
