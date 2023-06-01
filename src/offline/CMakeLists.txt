add_executable(FakeOffline FakeOfflineMain.cpp FakeOffline.cpp FakeOffline.h FakeOfflineGate.h)
target_include_directories(FakeOffline PUBLIC ../)
target_link_libraries(FakeOffline uint128)
