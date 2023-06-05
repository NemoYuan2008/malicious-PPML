#include <fstream>
#include <array>
#include <memory>
#include <filesystem>
#include <type_traits>
#include <boost/test/unit_test.hpp>

#include "share/Spdz2kShare.h"
#include "offline/FakeCircuit.h"
#include "protocols/Circuit.h"

using std::array;
using std::shared_ptr;
using std::ifstream;
using boost::unit_test::depends_on;

namespace {
    auto path = std::filesystem::temp_directory_path();
}

struct FakeCircuitFixture32 {
    std::array<std::ofstream, 2> outFiles;
    FakeOffline<32, 32, 2> offline;
    FakeCircuit<Spdz2kShare32, 2> fakeCircuit{outFiles, offline};
    std::shared_ptr<FakeInputGate<Spdz2kShare32, 2>> fx, fy, fz;
    std::shared_ptr<FakeAdditionGate<Spdz2kShare32, 2>> fa, fc;
    std::shared_ptr<FakeMultiplicationGate<Spdz2kShare32, 2>> fb, fd;


    FakeCircuitFixture32()
            : outFiles{std::ofstream(path / "0_32.txt"), std::ofstream(path / "1_32.txt")},
              offline(),
              fakeCircuit(outFiles, offline) {
        fx = fakeCircuit.input();
        fy = fakeCircuit.input();
        fz = fakeCircuit.input();

        fa = fakeCircuit.add(fx, fy);
        fb = fakeCircuit.multiply(fa, fx);
        fc = fakeCircuit.add(fz, fb);
        fd = fakeCircuit.multiply(fa, fc);
    }
};

struct FakeCircuitFixture64 {
    std::array<std::ofstream, 2> outFiles;
    FakeOffline<64, 64, 2> offline;
    FakeCircuit<Spdz2kShare64, 2> fakeCircuit{outFiles, offline};
    std::shared_ptr<FakeInputGate<Spdz2kShare64, 2>> fx, fy, fz;
    std::shared_ptr<FakeAdditionGate<Spdz2kShare64, 2>> fa, fc;
    std::shared_ptr<FakeMultiplicationGate<Spdz2kShare64, 2>> fb, fd;


    FakeCircuitFixture64()
            : outFiles{std::ofstream(path / "0_64.txt"), std::ofstream(path / "1_64.txt")},
              offline(),
              fakeCircuit(outFiles, offline) {
        fx = fakeCircuit.input();
        fy = fakeCircuit.input();
        fz = fakeCircuit.input();

        fa = fakeCircuit.add(fx, fy);
        fb = fakeCircuit.multiply(fa, fx);
        fc = fakeCircuit.add(fz, fb);
        fd = fakeCircuit.multiply(fa, fc);
    }
};

BOOST_AUTO_TEST_SUITE(FakeCircuitTest)

    BOOST_FIXTURE_TEST_CASE(FakeCircuit32, FakeCircuitFixture32) {
        fakeCircuit.addEndpoint(fd);
//        fakeCircuit.addEndpoint(fb);
        fakeCircuit.runOffline();

        BOOST_REQUIRE((std::is_same_v<decltype(fa->getLambdaClear()), uint32_t>));

        //Check lambdaClear
        BOOST_CHECK_EQUAL(fa->getLambdaClear(), fx->getLambdaClear() + fy->getLambdaClear());
        BOOST_CHECK_EQUAL(fb->getLambdaXyClear(), fa->getLambdaClear() * fx->getLambdaClear());
        BOOST_CHECK_EQUAL(fc->getLambdaClear(), fz->getLambdaClear() + fb->getLambdaClear());
        BOOST_CHECK_EQUAL(fd->getLambdaXyClear(), fa->getLambdaClear() * fc->getLambdaClear());

        //Check lambdaShares
        BOOST_CHECK_EQUAL(offline.openShares(fa->getLambdaShares()), fa->getLambdaClear());
        BOOST_CHECK_EQUAL(offline.openShares(fb->getLambdaShares()), fb->getLambdaClear());
        BOOST_CHECK_EQUAL(offline.openShares(fc->getLambdaShares()), fc->getLambdaClear());
        BOOST_CHECK_EQUAL(offline.openShares(fd->getLambdaShares()), fd->getLambdaClear());
        BOOST_CHECK_EQUAL(offline.openShares(fb->getLambdaXyShares()), fb->getLambdaXyClear());
        BOOST_CHECK_EQUAL(offline.openShares(fd->getLambdaXyShares()), fd->getLambdaXyClear());
    }

    BOOST_FIXTURE_TEST_CASE(FakeCircuit64, FakeCircuitFixture64) {
        fakeCircuit.addEndpoint(fd);
//        fakeCircuit.addEndpoint(fb);
        fakeCircuit.runOffline();

        BOOST_REQUIRE((std::is_same_v<decltype(fa->getLambdaClear()), uint64_t>));

        //Check lambdaClear
        BOOST_CHECK_EQUAL(fa->getLambdaClear(), fx->getLambdaClear() + fy->getLambdaClear());
        BOOST_CHECK_EQUAL(fb->getLambdaXyClear(), fa->getLambdaClear() * fx->getLambdaClear());
        BOOST_CHECK_EQUAL(fc->getLambdaClear(), fz->getLambdaClear() + fb->getLambdaClear());
        BOOST_CHECK_EQUAL(fd->getLambdaXyClear(), fa->getLambdaClear() * fc->getLambdaClear());

        //Check lambdaShares
        BOOST_CHECK_EQUAL(offline.openShares(fa->getLambdaShares()), fa->getLambdaClear());
        BOOST_CHECK_EQUAL(offline.openShares(fb->getLambdaShares()), fb->getLambdaClear());
        BOOST_CHECK_EQUAL(offline.openShares(fc->getLambdaShares()), fc->getLambdaClear());
        BOOST_CHECK_EQUAL(offline.openShares(fd->getLambdaShares()), fd->getLambdaClear());
        BOOST_CHECK_EQUAL(offline.openShares(fb->getLambdaXyShares()), fb->getLambdaXyClear());
        BOOST_CHECK_EQUAL(offline.openShares(fd->getLambdaXyShares()), fd->getLambdaXyClear());
    }

BOOST_AUTO_TEST_SUITE_END()
