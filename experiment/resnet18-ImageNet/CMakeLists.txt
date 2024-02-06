cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
list(APPEND CMAKE_PREFIX_PATH "/home/ysx/Documents/github/libtorch")

add_executable(resnet18_ImageNet_party0 resnet18Party0.cpp resnet18Config.h)
add_executable(resnet18_ImageNet_party1 resnet18Party1.cpp resnet18Config.h)
add_executable(resnet18_ImageNet_fakeOffline resnet18FakeOffline.cpp resnet18Config.h)

target_link_libraries(resnet18_ImageNet_party0 network Eigen3::Eigen "${TORCH_LIBRARIES}")
target_link_libraries(resnet18_ImageNet_party1 network Eigen3::Eigen "${TORCH_LIBRARIES}")
target_link_libraries(resnet18_ImageNet_fakeOffline Eigen3::Eigen "${TORCH_LIBRARIES}")
