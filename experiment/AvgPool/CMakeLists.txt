add_executable(AvgPool_party0 AvgPoolParty0.cpp AvgPoolConfig.h)
add_executable(AvgPool_party1 AvgPoolParty1.cpp AvgPoolConfig.h)
add_executable(AvgPool_fakeOffline AvgPoolFakeOffline.cpp AvgPoolConfig.h)

target_link_libraries(AvgPool_party0 network Eigen3::Eigen)
target_link_libraries(AvgPool_party1 network Eigen3::Eigen)
target_link_libraries(AvgPool_fakeOffline Eigen3::Eigen)