#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <type_traits>
#include <boost/test/unit_test.hpp>
#include "share/Spdz2kShare.h"
#include "utils/rand.h"

using std::is_same_v;


struct Spdz2kShareFixture {
    Spdz2kShare32 s32{getRand<uint32_t>(), getRand<uint32_t>()};
    Spdz2kShare64 s64{getRand<uint32_t>(), getRand<uint32_t>()};
    uint32_t c32{getRand<uint32_t>()};
    uint64_t c64{getRand<uint64_t>()};
};

BOOST_AUTO_TEST_CASE(Type) {
    BOOST_CHECK((is_same_v<Spdz2kShare32::KSType, uint64_t>));
    BOOST_CHECK((is_same_v<Spdz2kShare32::KType, uint32_t>));
    BOOST_CHECK((is_same_v<Spdz2kShare32::SType, uint32_t>));
    BOOST_CHECK((is_same_v<Spdz2kShare64::KSType, __uint128_t>));
    BOOST_CHECK((is_same_v<Spdz2kShare64::KType, uint64_t>));
    BOOST_CHECK((is_same_v<Spdz2kShare64::SType, uint64_t>));
}

BOOST_AUTO_TEST_CASE(AdditionSubstraction) {
    BOOST_CHECK_EQUAL(Spdz2kShare32(12, 34) + Spdz2kShare32(21, 43), Spdz2kShare32(33, 77));
    BOOST_CHECK_EQUAL(Spdz2kShare64(12, 34) + Spdz2kShare64(21, 43), Spdz2kShare64(33, 77));
    BOOST_CHECK_EQUAL(Spdz2kShare32(21, 43) - Spdz2kShare32(12, 34), Spdz2kShare32(9, 9));
    BOOST_CHECK_EQUAL(Spdz2kShare64(21, 43) - Spdz2kShare64(12, 34), Spdz2kShare64(9, 9));
}

BOOST_FIXTURE_TEST_CASE(MultiplyByConstant, Spdz2kShareFixture) {
    auto r32 = s32 * c32;
    BOOST_CHECK_EQUAL(s32.getXi() * c32, r32.getXi());
    BOOST_CHECK_EQUAL(s32.getMi() * c32, r32.getMi());
    s32 *= c32;
    BOOST_CHECK_EQUAL(s32, r32);

    auto r64 = s64 * c64;
    BOOST_CHECK(s64.getXi() * c64 == r64.getXi());
    BOOST_CHECK(s64.getMi() * c64 == r64.getMi());
    s64 *= c64;
    BOOST_CHECK_EQUAL(s64, r64);
}
