add_executable(conv2D_party0 Conv2DParty0.cpp Conv2DConfig.h ../../src/utils/linear_algebra.h)
add_executable(conv2D_party1 Conv2DParty1.cpp Conv2DConfig.h ../../src/utils/linear_algebra.h)
add_executable(conv2D_fakeOffline Conv2DFakeOffline.cpp Conv2DConfig.h ../../src/utils/linear_algebra.h)

target_link_libraries(conv2D_party0 network Eigen3::Eigen)
target_link_libraries(conv2D_party1 network Eigen3::Eigen)
target_link_libraries(conv2D_fakeOffline Eigen3::Eigen)
