cmake_minimum_required(VERSION 3.25)
project(malicious_PPML)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS thread)
include_directories(src ${Boost_INCLUDE_DIR})

add_executable(malicious_PPML
        main.cpp src/protocols/Gate.h src/utils/rand.h src/protocols/InputGate.h src/protocols/AdditionGate.h src/protocols/MultiplicationGate.h src/protocols/Circuit.h src/utils/Party.h)

target_link_libraries(malicious_PPML network)

add_subdirectory(src/offline)

add_subdirectory(tests)

add_subdirectory(src/networking)


add_executable(party0 party0.cpp)
add_executable(party1 party1.cpp)
target_link_libraries(party0 network)
target_link_libraries(party1 network)

