#include <iostream>
#include <fstream>
#include <filesystem>

#include "share/Spdz2kShare.h"
#include "offline/FakeOffline.h"
#include "offline/FakeCircuit.h"

#include "FCConfig.h"


int main() {
    auto path = std::filesystem::temp_directory_path();
    std::cout << path << '\n';

    std::array<std::ofstream, 2> files{
            std::ofstream(path / "0.txt"),
            std::ofstream(path / "1.txt")
    };

    FakeOffline<64, 64, 2> offline(files);
    FakeCircuit<Spdz2kShare64, 2> circuit(files, offline);

    for (int i = 0; i < times; ++i) {
        auto fc = circuit.input(0, linear, fc_size);
        auto input = circuit.input(0, fc_size, batch_size);
        auto a = circuit.multiplyTrunc(fc, input);
        auto o = circuit.output(a);
        circuit.addEndpoint(o);
    }

    circuit.runOffline();

    return 0;
}