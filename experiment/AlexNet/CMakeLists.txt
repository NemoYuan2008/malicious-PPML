cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
list(APPEND CMAKE_PREFIX_PATH "/home/ysx/Documents/github/libtorch")

add_executable(AlexNet_party0 AlexNetParty0.cpp AlexNetConfig.h)
add_executable(AlexNet_party1 AlexNetParty1.cpp AlexNetConfig.h)
add_executable(AlexNet_fakeOffline AlexNetFakeOffline.cpp AlexNetConfig.h)

target_link_libraries(AlexNet_party0 network Eigen3::Eigen "${TORCH_LIBRARIES}")
target_link_libraries(AlexNet_party1 network Eigen3::Eigen "${TORCH_LIBRARIES}")
target_link_libraries(AlexNet_fakeOffline Eigen3::Eigen "${TORCH_LIBRARIES}")

