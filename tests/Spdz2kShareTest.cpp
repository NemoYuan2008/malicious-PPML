#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include "share/Spdz2kShare.h"

BOOST_AUTO_TEST_SUITE(Spdz2kTest)

    BOOST_AUTO_TEST_CASE(Size) {
        BOOST_CHECK_EQUAL(sizeof(Spdz2kShare32::KSType), sizeof(uint64_t));
        BOOST_CHECK_EQUAL(sizeof(Spdz2kShare32::KType), sizeof(uint32_t));
        BOOST_CHECK_EQUAL(sizeof(Spdz2kShare32::SType), sizeof(uint32_t));
        BOOST_CHECK_EQUAL(sizeof(Spdz2kShare64::KSType), sizeof(__uint128_t));
        BOOST_CHECK_EQUAL(sizeof(Spdz2kShare64::KType), sizeof(uint64_t));
        BOOST_CHECK_EQUAL(sizeof(Spdz2kShare64::SType), sizeof(uint64_t));
    }

    BOOST_AUTO_TEST_CASE(Addition) {
        BOOST_CHECK_EQUAL(Spdz2kShare32(12, 34) + Spdz2kShare32(21, 43), Spdz2kShare32(33, 77));
    }

BOOST_AUTO_TEST_SUITE_END()
