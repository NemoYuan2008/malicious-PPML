#include <numeric>
#include <boost/test/unit_test.hpp>

#include "utils/rand.h"
#include "share/uint128.h"
#include "offline/FakeOffline.h"


struct FakeOfflineFixture {
    FakeOffline<32, 32, 2> f32_2;
    FakeOffline<64, 64, 2> f64_2;

    FakeOfflineFixture(): f32_2(getRand<uint32_t>()), f64_2(getRand<uint64_t>()) {}
};

BOOST_FIXTURE_TEST_CASE(SplitNTest, FakeOfflineFixture) {
    auto x = getRand<KSType_t<32, 32>>();
    decltype(x) zero_x = 0;
    auto x_split = f32_2.splitN(x);

    auto y = getRand<KSType_t<64, 64>>();
    decltype(y) zero_y = 0;
    auto y_split = f64_2.splitN(y);

    BOOST_CHECK(std::accumulate(x_split.begin(), x_split.end(), zero_x) == x);
    BOOST_CHECK(std::accumulate(y_split.begin(), y_split.end(), zero_y) == y);
}

