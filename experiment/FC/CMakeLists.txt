add_executable(FC_party0 FCParty0.cpp FCConfig.h)
add_executable(FC_party1 FCParty1.cpp FCConfig.h)
add_executable(FC_fakeOffline FCFakeOffline.cpp FCConfig.h)

target_link_libraries(FC_party0 network Eigen3::Eigen)
target_link_libraries(FC_party1 network Eigen3::Eigen)
target_link_libraries(FC_fakeOffline Eigen3::Eigen)