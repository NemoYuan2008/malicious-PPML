cmake_minimum_required(VERSION 3.25)
project(malicious_PPML)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS thread)
find_package(Eigen3 REQUIRED NO_MODULE)

include_directories(src ${Boost_INCLUDE_DIR})

add_executable(malicious_PPML
        main.cpp src/protocols/Gate.h src/utils/rand.h src/protocols/InputGate.h src/protocols/AdditionGate.h src/protocols/MultiplicationGate.h src/protocols/Circuit.h src/utils/Party.h src/utils/linear_algebra.h)

target_link_libraries(malicious_PPML network Eigen3::Eigen)

add_subdirectory(src/offline)

add_subdirectory(tests)

add_subdirectory(src/networking)


add_executable(party0 party0.cpp src/utils/linear_algebra.h)
add_executable(party1 party1.cpp src/utils/linear_algebra.h)
target_link_libraries(party0 network Eigen3::Eigen)
target_link_libraries(party1 network Eigen3::Eigen)

