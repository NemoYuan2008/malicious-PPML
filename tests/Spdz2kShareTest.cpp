#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include "share/Spdz2kShare.h"

BOOST_AUTO_TEST_SUITE(Spdz2kTest)

    BOOST_AUTO_TEST_CASE(Addition) {
        BOOST_CHECK_EQUAL(Spdz2kShare32(12, 34) + Spdz2kShare32(21, 43), Spdz2kShare32(33, 77));
    }

BOOST_AUTO_TEST_SUITE_END()
