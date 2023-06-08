#include <numeric>
#include <type_traits>
#include <boost/test/unit_test.hpp>

#include "utils/rand.h"
#include "utils/uint128.h"
#include "offline/FakeOffline.h"


struct FakeOfflineFixture {
    FakeOffline<32, 32, 2> f32_2;
    FakeOffline<64, 64, 2> f64_2;
};

BOOST_FIXTURE_TEST_SUITE(FakeOfflineTest, FakeOfflineFixture)

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

    BOOST_FIXTURE_TEST_CASE(GenerateShareTest32, FakeOfflineFixture) {
        auto clear = getRand<uint32_t>();
        auto shares = f32_2.generateShares(clear);
        auto sumShare = std::accumulate(shares.begin(), shares.end(), Spdz2kShare32());
        auto opened = f32_2.openShares(shares);

        BOOST_CHECK_EQUAL(clear, static_cast<uint32_t>(sumShare.getXi()));
        BOOST_CHECK_EQUAL(clear, opened);
        BOOST_CHECK((std::is_same_v<decltype(clear), decltype(opened)>));
        BOOST_CHECK_EQUAL(sumShare.getMi(), f32_2.getGlobalKey() * sumShare.getXi());
    }

    BOOST_FIXTURE_TEST_CASE(GenerateShareTest64, FakeOfflineFixture) {
        auto clear = getRand<uint64_t>();
        auto shares = f64_2.generateShares(clear);
        auto sumShare = std::accumulate(shares.begin(), shares.end(), Spdz2kShare64());
        auto opened = f64_2.openShares(shares);

        BOOST_CHECK_EQUAL(clear, static_cast<uint64_t>(sumShare.getXi()));
        BOOST_CHECK_EQUAL(clear, opened);
        BOOST_CHECK((std::is_same_v<decltype(clear), decltype(opened)>));
        BOOST_CHECK(sumShare.getMi() == f64_2.getGlobalKey() * sumShare.getXi());
    }

BOOST_AUTO_TEST_SUITE_END()
