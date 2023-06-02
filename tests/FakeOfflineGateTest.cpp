#include <iostream>
#include <memory>
#include <boost/test/unit_test.hpp>

#include "offline/FakeOfflineGate.h"
#include "offline/FakeOffline.h"


BOOST_AUTO_TEST_CASE(FakeGate32) {
    FakeOffline<32, 32, 2> offline;

    //a = x + y, b = a * x, c = z + b, d = a * c
    auto x = std::make_shared<FakeInputGate<Spdz2kShare32, 2>>(std::cout, offline);
    auto y = std::make_shared<FakeInputGate<Spdz2kShare32, 2>>(std::cout, offline);
    auto z = std::make_shared<FakeInputGate<Spdz2kShare32, 2>>(std::cout, offline);

    auto a = std::make_shared<FakeAdditionGate<Spdz2kShare32, 2>>(x, y);
    auto b = std::make_shared<FakeMultiplicationGate<Spdz2kShare32, 2>>(a, x);
    auto c = std::make_shared<FakeAdditionGate<Spdz2kShare32, 2>>(z, b);
    auto d = std::make_shared<FakeMultiplicationGate<Spdz2kShare32, 2>>(a, c);

    d->runOffline();

    //Check lambdaClear
    BOOST_CHECK_EQUAL(a->getLambdaClear(), x->getLambdaClear() + y->getLambdaClear());
    BOOST_CHECK_EQUAL(b->getLambdaXyClear(), a->getLambdaClear() * x->getLambdaClear());
    BOOST_CHECK_EQUAL(c->getLambdaClear(), z->getLambdaClear() + b->getLambdaClear());
    BOOST_CHECK_EQUAL(d->getLambdaXyClear(), a->getLambdaClear() * c->getLambdaClear());

    //Check lambdaShares
    BOOST_CHECK_EQUAL(offline.openShares(a->getLambdaShares()), a->getLambdaClear());
    BOOST_CHECK_EQUAL(offline.openShares(b->getLambdaShares()), b->getLambdaClear());
    BOOST_CHECK_EQUAL(offline.openShares(c->getLambdaShares()), c->getLambdaClear());
    BOOST_CHECK_EQUAL(offline.openShares(d->getLambdaShares()), d->getLambdaClear());
    BOOST_CHECK_EQUAL(offline.openShares(b->getLambdaXyShares()), b->getLambdaXyClear());
    BOOST_CHECK_EQUAL(offline.openShares(d->getLambdaXyShares()), d->getLambdaXyClear());
}
