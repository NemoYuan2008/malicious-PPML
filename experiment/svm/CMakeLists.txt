add_executable(svm_party0 SVMParty0.cpp SVMConfig.h ../../src/utils/fixedPoint.h)
add_executable(svm_party1 SVMParty1.cpp SVMConfig.h ../../src/utils/fixedPoint.h)
add_executable(svm_fakeOffline SVMFakeOffline.cpp SVMConfig.h ../../src/utils/fixedPoint.h)

target_link_libraries(svm_party0 network Eigen3::Eigen)
target_link_libraries(svm_party1 network Eigen3::Eigen)
target_link_libraries(svm_fakeOffline Eigen3::Eigen)