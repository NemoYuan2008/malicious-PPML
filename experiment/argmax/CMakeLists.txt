add_executable(argmax_party0 ArgmaxParty0.cpp ArgmaxConfig.h)
add_executable(argmax_party1 ArgmaxParty1.cpp ArgmaxConfig.h)
add_executable(argmax_fakeOffline ArgmaxFakeOffline.cpp ArgmaxConfig.h)

target_link_libraries(argmax_party0 network Eigen3::Eigen)
target_link_libraries(argmax_party1 network Eigen3::Eigen)
target_link_libraries(argmax_fakeOffline Eigen3::Eigen)