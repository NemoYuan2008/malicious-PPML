#include <iostream>
#include <memory>
#include <array>
#include <boost/test/unit_test.hpp>

#include "offline/FakeGate.h"
#include "offline/FakeOffline.h"

BOOST_AUTO_TEST_SUITE(FakeGateNoCircuitTest)

    BOOST_AUTO_TEST_CASE(FakeGate32) {
        FakeOffline<32, 32, 2> offline;
        std::array<std::ostream *, 2> os{&std::cout, &std::cout};

        //a = x + y, b = a * x, c = z + b, d = a * c
        auto x = std::make_shared<FakeInputGate<Spdz2kShare32, 2>>(os, offline);
        auto y = std::make_shared<FakeInputGate<Spdz2kShare32, 2>>(os, offline);
        auto z = std::make_shared<FakeInputGate<Spdz2kShare32, 2>>(os, offline);

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

        //Check lambdaShr
        BOOST_CHECK_EQUAL(offline.openShares(a->getLambdaShr()), a->getLambdaClear());
        BOOST_CHECK_EQUAL(offline.openShares(b->getLambdaShr()), b->getLambdaClear());
        BOOST_CHECK_EQUAL(offline.openShares(c->getLambdaShr()), c->getLambdaClear());
        BOOST_CHECK_EQUAL(offline.openShares(d->getLambdaShr()), d->getLambdaClear());
        BOOST_CHECK_EQUAL(offline.openShares(b->getLambdaXyShr()), b->getLambdaXyClear());
        BOOST_CHECK_EQUAL(offline.openShares(d->getLambdaXyShr()), d->getLambdaXyClear());
    }


    BOOST_AUTO_TEST_CASE(FakeGate64) {
        FakeOffline<64, 64, 2> offline;
        std::array<std::ostream *, 2> os{&std::cout, &std::cout};

        //a = x + y, b = a * x, c = z + b, d = a * c
        auto x = std::make_shared<FakeInputGate<Spdz2kShare64, 2>>(os, offline);
        auto y = std::make_shared<FakeInputGate<Spdz2kShare64, 2>>(os, offline);
        auto z = std::make_shared<FakeInputGate<Spdz2kShare64, 2>>(os, offline);

        auto a = std::make_shared<FakeAdditionGate<Spdz2kShare64, 2>>(x, y);
        auto b = std::make_shared<FakeMultiplicationGate<Spdz2kShare64, 2>>(a, x);
        auto c = std::make_shared<FakeAdditionGate<Spdz2kShare64, 2>>(z, b);
        auto d = std::make_shared<FakeMultiplicationGate<Spdz2kShare64, 2>>(a, c);

        d->runOffline();

        //Check lambdaClear
        BOOST_CHECK_EQUAL(a->getLambdaClear(), x->getLambdaClear() + y->getLambdaClear());
        BOOST_CHECK_EQUAL(b->getLambdaXyClear(), a->getLambdaClear() * x->getLambdaClear());
        BOOST_CHECK_EQUAL(c->getLambdaClear(), z->getLambdaClear() + b->getLambdaClear());
        BOOST_CHECK_EQUAL(d->getLambdaXyClear(), a->getLambdaClear() * c->getLambdaClear());

        //Check lambdaShr
        BOOST_CHECK_EQUAL(offline.openShares(a->getLambdaShr()), a->getLambdaClear());
        BOOST_CHECK_EQUAL(offline.openShares(b->getLambdaShr()), b->getLambdaClear());
        BOOST_CHECK_EQUAL(offline.openShares(c->getLambdaShr()), c->getLambdaClear());
        BOOST_CHECK_EQUAL(offline.openShares(d->getLambdaShr()), d->getLambdaClear());
        BOOST_CHECK_EQUAL(offline.openShares(b->getLambdaXyShr()), b->getLambdaXyClear());
        BOOST_CHECK_EQUAL(offline.openShares(d->getLambdaXyShr()), d->getLambdaXyClear());
    }

BOOST_AUTO_TEST_SUITE_END()
