#include <iostream>
#include "share/Spdz2kShare.h"
#include "offline/FakeOffline.h"
#include "protocols/GateOffline.h"
#include "utils/rand.h"

FakeOffline<32, 32, 2>::Shares w_shares, x_shares, b_shares, z_shares, wx_shares;


void fakeOffline() {
    FakeOffline<32, 32, 2> offline(0x1);
    FakeOffline<32, 32, 2>::KType w_clear = 1, x_clear = 2, b_clear = 3, z_clear = 4;
    // TODO: shares are not correct
    w_shares = offline.generateShares(w_clear);
    x_shares = offline.generateShares(x_clear);
    b_shares = offline.generateShares(b_clear);
    z_shares = offline.generateShares(z_clear);
    wx_shares = offline.getSpdz2kTriple(w_clear, x_clear);
}

void party1() {
    auto w_share = w_shares[0];
    auto x_share = x_shares[0];
    auto b_share = b_shares[0];
    MultiplicationOfflineFake<32, 32> mult;
    mult.runFakeOffline(z_shares[0], wx_shares[0]);
    AdditionOffline<32, 32> add;
    add.runOffline(mult.delta_z, b_share);
}

void party2() {
    auto w_share = w_shares[1];
    auto x_share = x_shares[1];
    auto b_share = b_shares[1];
    MultiplicationOfflineFake<32, 32> mult;
    mult.runFakeOffline(z_shares[0], wx_shares[0]);
    AdditionOffline<32, 32> add;
    add.runOffline(mult.delta_z, b_share);
}

int main() {
    FakeOffline<32, 32, 2>::KType clear = 2;
    FakeOffline<32, 32, 2> offline(0x1);
    auto shares = offline.generateShares(clear);
    std::cout << shares[0] << '\n'
              << shares[1] << '\n';

    KSType_t<32, 32> x = 10;
    auto x_split = FakeOffline<32, 32, 2>::splitN(x);
    auto sum = std::accumulate(x_split.begin(), x_split.end(), 0UL);
    std::cout << x_split[0] << ' ' << x_split[1] << ' ' << sum << '\n';

//    auto y = getRand<KSType_t<32, 32>>();
//    auto y_split = FakeOffline<32, 32, 2>::splitN(x);
//    auto sum = std::accumulate(x_split.begin(), x_split.end());
//    BOOST_CHECK_EQUAL(std::accumulate(x_split.begin(), x_split.end()), x);

    return 0;
}
