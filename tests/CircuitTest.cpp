#include <memory>
#include <boost/test/unit_test.hpp>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "protocols/InputGate.h"
#include "utils/rand.h"

using std::array;
using std::shared_ptr;
using std::ifstream;
using boost::unit_test::depends_on;

namespace {
    auto path = std::filesystem::temp_directory_path();
}

template<typename ShrType>
struct CircuitFixture {
    array<ifstream, 2> inFiles{ifstream(path / "0.txt"), ifstream(path / "1.txt")};
    array<Circuit<ShrType>, 2> circuit;
    array<shared_ptr<InputGate<ShrType>>, 2> x, y, z;
    array<shared_ptr<AdditionGate<ShrType>>, 2> a, c;
    array<shared_ptr<MultiplicationGate<ShrType>>, 2> b, d;

    CircuitFixture() {
        for (int i = 0; i < 2; ++i) {
            x[i] = circuit[i].input();
            y[i] = circuit[i].input();
            z[i] = circuit[i].input();

            a[i] = circuit[i].add(x[i], y[i]);
            b[i] = circuit[i].multiply(a[i], x[i]);
            c[i] = circuit[i].add(z[i], b[i]);
            d[i] = circuit[i].multiply(a[i], c[i]);
        }
    }
};

BOOST_AUTO_TEST_SUITE(CircuitTest)

    BOOST_FIXTURE_TEST_CASE(Circuit32, CircuitFixture<Spdz2kShare32>, *depends_on("FakeCircuitTest/FakeCircuit32")) {
        for (int i = 0; i < 2; ++i) {
            circuit[i].addEndpoint(d[i]);
            circuit[i].readOfflineFromFile(inFiles[i]);
        }

        //Check addition gates
        BOOST_CHECK(a[0]->getLambdaShr() == x[0]->getLambdaShr() + y[0]->getLambdaShr());
        BOOST_CHECK(a[1]->getLambdaShr() == x[1]->getLambdaShr() + y[1]->getLambdaShr());
        BOOST_CHECK(c[0]->getLambdaShr() == z[0]->getLambdaShr() + b[0]->getLambdaShr());
        BOOST_CHECK(c[1]->getLambdaShr() == z[1]->getLambdaShr() + b[1]->getLambdaShr());

        //Check multiplication gates

    }

BOOST_AUTO_TEST_SUITE_END()
