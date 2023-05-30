#include <memory>
#include <boost/test/unit_test.hpp>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "protocols/InputGate.h"
#include "utils/rand.h"

struct CircuitFixture32 {
    uint32_t xClear{getRand<uint32_t>()}, yClear{getRand<uint32_t>()};
    std::shared_ptr<InputGate<Spdz2kShare32>> x, y;
    Circuit<Spdz2kShare32> circuit;

    CircuitFixture32() : xClear{getRand<uint32_t>()}, yClear{getRand<uint32_t>()},
                         x(std::make_shared<InputGate<Spdz2kShare32>>()),
                         y(std::make_shared<InputGate<Spdz2kShare32>>()) {
        xClear = 10, yClear = 20;
        x->setDeltaClear(xClear);
        y->setDeltaClear(yClear);
    }
};


BOOST_FIXTURE_TEST_CASE(CircuitAdd32, CircuitFixture32) {
    auto a = circuit.add(x, y);
    auto b = circuit.add(a, x);
    auto c = circuit.add(a, b);
    circuit.addEndpoint(b);
    circuit.addEndpoint(c);
    circuit.runOnline();

    auto aClear = xClear + yClear, bClear = aClear + xClear, cClear = aClear + bClear;
    BOOST_CHECK_EQUAL(a->getDeltaClear(), aClear);
    BOOST_CHECK_EQUAL(b->getDeltaClear(), bClear);
    BOOST_CHECK_EQUAL(c->getDeltaClear(), cClear);
}
