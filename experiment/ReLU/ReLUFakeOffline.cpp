#include <iostream>
#include <fstream>
#include <filesystem>

#include "share/Spdz2kShare.h"
#include "offline/FakeOffline.h"
#include "offline/FakeCircuit.h"

#include "ReLUConfig.h"


int main() {
    auto path = std::filesystem::temp_directory_path();
    std::cout << path << '\n';

    std::array<std::ofstream, 2> files{
            std::ofstream(path / "0.txt"),
            std::ofstream(path / "1.txt")
    };

    FakeOffline<32, 32, 2> offline(files);
    FakeCircuit<Spdz2kShare32, 2> circuit(files, offline);

//    for (int i = 0; i < times; ++i) {
    auto x = circuit.input(0, rows, cols);
    auto a = circuit.relu(x);
    auto o = circuit.output(a);
    circuit.addEndpoint(o);
//    }

    circuit.runOffline();

    return 0;
}