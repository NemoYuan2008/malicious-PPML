#include <iostream>
#include <filesystem>
#include <vector>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/rand.h"
#include "utils/Party.h"
#include "utils/benchmark.h"
#include "utils/ioHelper.h"
#include "FCConfig.h"
#include <chrono>
using namespace std::chrono;
std::vector<double> generateRandIn(uint32_t rows, uint32_t cols){
    std::vector<double> ret(rows*cols);
    for (int i = 0; i < rows*cols; ++i) {
        ret[i] = rand();
    }
    return ret;
}
int main() {
    auto path = std::filesystem::temp_directory_path();
    auto fc_input = double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(linear,fc_size));
    auto input_ = double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(fc_size,batch_size));

    Party<Spdz2kShare64> party(0, 2, (path / "0.txt").string());
    Circuit<Spdz2kShare64> circuit(&party);
    for (int i = 0; i < times; ++i) {
        auto fc = circuit.input(0, linear, fc_size);
        auto input = circuit.input(0, fc_size, batch_size);
        auto a = circuit.multiplyTrunc(fc, input);
        auto o = circuit.output(a);
        circuit.addEndpoint(o);

        fc->setInput(fc_input);
        input->setInput(input_);
    }
    circuit.readOfflineFromFile();
    circuit.shakeHand();
    std::cout << "timer start:" << std::endl;
    auto start = std::chrono::steady_clock::now();
//    std::cout << benchmark([&]() { circuit.runOnline(); }) << "ms\n";
    circuit.runOnline();
    auto finish = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() << "ms" << std::endl;

//    circuit.runOnline();

//    std::cout << "Result: \n";
//    printVector(o->getClear());

    return 0;
}