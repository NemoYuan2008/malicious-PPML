add_executable(FakeOffline FakeOfflineMain.cpp FakeOffline.h FakeOfflineGate.h FakeOfflineBase.h FakeCircuit.h)
target_include_directories(FakeOffline PUBLIC ../)
target_link_libraries(FakeOffline uint128)
