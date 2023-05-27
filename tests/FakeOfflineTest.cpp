#include <numeric>
#include <boost/test/unit_test.hpp>

#include "share/uint128.h"
#include "offline/FakeOffline.h"
#include "utils/rand.h"

BOOST_AUTO_TEST_SUITE(FakeOfflineTest)

    BOOST_AUTO_TEST_CASE(SplitN) {
        auto x = getRand<KSType_t<32, 32>>();
        decltype(x) zero_x = 0;
        auto x_split = FakeOffline<32, 32, 2>::splitN(x);
        BOOST_CHECK_EQUAL(std::accumulate(x_split.begin(), x_split.end(), zero_x), x);

        auto y = getRand<KSType_t<64, 64>>();
        decltype(y) zero_y = 0;
        auto y_split = FakeOffline<64, 64, 2>::splitN(x);
        BOOST_CHECK(std::accumulate(y_split.begin(), y_split.end(), zero_y) == y);
    }

BOOST_AUTO_TEST_SUITE_END()