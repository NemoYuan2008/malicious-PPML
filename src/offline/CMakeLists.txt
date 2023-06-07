add_executable(FakeOffline FakeOfflineMain.cpp FakeOffline.h FakeGate.h FakeOfflineBase.h FakeCircuit.h ../utils/Party.h)
target_include_directories(FakeOffline PUBLIC ../)
target_link_libraries(FakeOffline uint128)
