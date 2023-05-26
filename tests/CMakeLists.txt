find_package(Boost REQUIRED COMPONENTS unit_test_framework)

include_directories(${Boost_INCLUDE_DIR})
include_directories(../src)
add_executable(tests Spdz2kShareTest.cpp ../src/share/Spdz2kShare.cpp)
target_link_libraries(tests ${Boost_LIBRARIES} uint128)
