#include <fstream>
#include <array>
#include <memory>
#include <filesystem>
#include <boost/test/unit_test.hpp>

#include "share/Spdz2kShare.h"
#include "offline/FakeCircuit.h"

namespace {
    auto path = std::filesystem::temp_directory_path();
}

struct FakeCircuitFixture {
    std::array<std::ofstream, 2> files;
    FakeOffline<32, 32, 2> offline;
    FakeCircuit<Spdz2kShare32, 2> circuit{files, offline};
    std::shared_ptr<FakeInputGate<Spdz2kShare32, 2>> x, y, z;
    std::shared_ptr<FakeAdditionGate<Spdz2kShare32, 2>> a, c;
    std::shared_ptr<FakeMultiplicationGate<Spdz2kShare32, 2>> b, d;

    FakeCircuitFixture()
            : files{std::ofstream(path / "0.txt"), std::ofstream(path / "1.txt")},
              offline(),
              circuit(files, offline) {
        x = circuit.input();
        y = circuit.input();
        z = circuit.input();

        a = circuit.add(x, y);
        b = circuit.multiply(a, x);
        c = circuit.add(z, b);
        d = circuit.multiply(a, c);
    }
};

BOOST_FIXTURE_TEST_SUITE(FakeCircuitTest, FakeCircuitFixture)

    BOOST_FIXTURE_TEST_CASE(FakeCircuit32, FakeCircuitFixture) {
        circuit.addEndpoint(d);
        circuit.addEndpoint(b);
        circuit.runOffline();

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

BOOST_AUTO_TEST_SUITE_END()
