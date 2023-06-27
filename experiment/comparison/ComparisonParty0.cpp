#include <iostream>
#include <filesystem>
#include <vector>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/Party.h"
#include "utils/ioHelper.h"
#include "ComparisonConfig.h"
#include "utils/benchmark.h"
#include "utils/fixedPoint.h"
using std::cout;
std::vector<double> generateRandIn(uint32_t rows, uint32_t cols){
    std::vector<double> ret(rows*cols);
    for (int i = 0; i < rows*cols; ++i) {
        ret[i] = rand();
    }
    return ret;
}
int main() {
    auto path = std::filesystem::temp_directory_path();
    std::vector<Spdz2kShare64::ClearType> xIn;
    xIn = {double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(classes,fc_size))};
    Party<Spdz2kShare64> party(0, 2, (path / "0.txt").string());
    Circuit<Spdz2kShare64> circuit(&party);
    for (int i = 0; i < times; ++i) {
        auto x = circuit.input(0, rows, cols);
        auto b = circuit.gtz(x);
        auto o = circuit.output(b);
        x->setInput(xIn);
        circuit.addEndpoint(o);
    }
    circuit.readOfflineFromFile();

    circuit.shakeHand();
    std::cout << benchmark([&]() { circuit.runOnline(); }) << "ms\n";

    return 0;
}
