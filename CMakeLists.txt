cmake_minimum_required(VERSION 3.25)
project(malicious_PPML)

set(CMAKE_CXX_STANDARD 17)
set(Boost_DEBUG ON)
set(BOOST_ROOT "/home/ysx/Downloads/boost_1_84_0")
set(BOOST_INCLUDEDIR "/home/ysx/Downloads/boost_1_84_0/boost")
set(BOOST_LIBRARYDIR "/home/ysx/Downloads/boost_1_84_0/stage/lib")
#set(Boost_INCLUDE_DIR "/usr/local/boost/include")
include_directories(src ${Boost_INCLUDE_DIR})
message(Boost_INCLUDE_DIR)

find_package(Boost REQUIRED COMPONENTS thread)
find_package(Eigen3 REQUIRED NO_MODULE)


add_executable(malicious_PPML
        main.cpp src/protocols/Gate.h src/utils/rand.h src/protocols/InputGate.h src/protocols/AdditionGate.h src/protocols/MultiplicationGate.h src/protocols/Circuit.h src/utils/Party.h src/utils/linear_algebra.h src/protocols/OutputGate.h src/protocols/MultiplyTruncGate.h src/utils/fixedPoint.h src/protocols/LtzGate.h src/utils/tensor.h src/protocols/Conv2DGate.h src/protocols/ReLUGate.h src/protocols/GtzGate.h src/protocols/ElemMultiplicationGate.h src/protocols/SubtractionGate.h src/protocols/SliceGate.h src/protocols/ArgmaxGate.h src/protocols/DummyInputGate.h src/protocols/MultiplyByConstantGate.h src/protocols/AddConstantGate.h src/protocols/AvgPool2DGate.h src/protocols/Conv2DTruncGate.h
        experiment/AlexNet/AlexNetConfig.h
        experiment/AlexNet/AlexNetFakeOffline.cpp
        experiment/AlexNet/AlexNetParty0.cpp
        experiment/AlexNet/AlexNetParty1.cpp)

#target_link_libraries(malicious_PPML network Eigen3::Eigen)

add_subdirectory(src/offline)

#add_subdirectory(tests)

add_subdirectory(src/networking)


add_executable(party0 party0.cpp src/utils/linear_algebra.h src/protocols/OutputGate.h src/protocols/MultiplyTruncGate.h src/utils/fixedPoint.h)
add_executable(party1 party1.cpp src/utils/linear_algebra.h src/protocols/OutputGate.h src/protocols/MultiplyTruncGate.h src/utils/fixedPoint.h)
target_link_libraries(party0 network Eigen3::Eigen)
target_link_libraries(party1 network Eigen3::Eigen)

add_subdirectory(experiment)
