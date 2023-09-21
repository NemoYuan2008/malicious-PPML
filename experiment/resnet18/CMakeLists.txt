cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
list(APPEND CMAKE_PREFIX_PATH "/home/ysx/Documents/github/libtorch")

add_executable(resnet18_party0 resnet18Party0.cpp resnet18Config.h)
add_executable(resnet18_party1 resnet18Party1.cpp resnet18Config.h)
add_executable(resnet18_fakeOffline resnet18FakeOffline.cpp resnet18Config.h)

target_link_libraries(resnet18_party0 network Eigen3::Eigen "${TORCH_LIBRARIES}")
target_link_libraries(resnet18_party1 network Eigen3::Eigen "${TORCH_LIBRARIES}")
target_link_libraries(resnet18_fakeOffline Eigen3::Eigen "${TORCH_LIBRARIES}")

