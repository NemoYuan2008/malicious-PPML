find_package(Boost REQUIRED COMPONENTS unit_test_framework)

include_directories(${Boost_INCLUDE_DIR})
include_directories(../src)
add_executable(tests Spdz2kShareTest.cpp FakeOfflineTest.cpp ../src/offline/FakeOffline.h ../src/offline/FakeOffline.cpp)
target_link_libraries(tests ${Boost_LIBRARIES} uint128 Spdz2kShare)
