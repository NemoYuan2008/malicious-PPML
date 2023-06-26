#include <iostream>
#include <filesystem>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/Party.h"
#include "utils/benchmark.h"

#include "FCConfig.h"


int main() {
    auto path = std::filesystem::temp_directory_path();

    Party<Spdz2kShare64> party(1, 2, (path / "1.txt").string());
    Circuit<Spdz2kShare64> circuit(&party);

    for (int i = 0; i < times; ++i) {
        auto fc = circuit.input(0, linear, fc_size);
        auto input = circuit.input(0, fc_size, batch_size);
        auto a = circuit.multiplyTrunc(fc, input);
        auto o = circuit.output(a);
        circuit.addEndpoint(o);
    }

    circuit.readOfflineFromFile();

    circuit.shakeHand();
    std::cout << "timer start:" << std::endl;
    auto start = std::chrono::steady_clock::now();
//    std::cout << benchmark([&]() { circuit.runOnline(); }) << "ms\n";
    circuit.runOnline();
    auto finish = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() << "ms" << std::endl;

    return 0;
}