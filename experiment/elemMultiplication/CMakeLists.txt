add_executable(Elem_party0 ElemParty0.cpp ElemConfig.h)
add_executable(Elem_party1 ElemParty1.cpp ElemConfig.h)
add_executable(Elem_fakeOffline ElemFakeOffline.cpp ElemConfig.h)

target_link_libraries(Elem_party0 network Eigen3::Eigen)
target_link_libraries(Elem_party1 network Eigen3::Eigen)
target_link_libraries(Elem_fakeOffline Eigen3::Eigen)