add_executable(ReLU_party0 ReLUParty0.cpp ReLUConfig.h)
add_executable(ReLU_party1 ReLUParty1.cpp ReLUConfig.h)
add_executable(ReLU_fakeOffline ReLUFakeOffline.cpp ReLUConfig.h)

target_link_libraries(ReLU_party0 network Eigen3::Eigen)
target_link_libraries(ReLU_party1 network Eigen3::Eigen)
target_link_libraries(ReLU_fakeOffline Eigen3::Eigen)