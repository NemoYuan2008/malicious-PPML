list(APPEND CMAKE_PREFIX_PATH "/home/zyx/Downloads/libtorch")

add_executable(lenet_party0 LenetParty0.cpp LenetConfig.h)
add_executable(lenet_party1 LenetParty1.cpp LenetConfig.h)
add_executable(lenet_fakeOffline LenetFakeOffline.cpp LenetConfig.h)

target_link_libraries(lenet_party0 network Eigen3::Eigen)
target_link_libraries(lenet_party1 network Eigen3::Eigen)
target_link_libraries(lenet_fakeOffline Eigen3::Eigen)