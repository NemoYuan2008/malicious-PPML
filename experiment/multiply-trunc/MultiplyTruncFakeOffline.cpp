#include <iostream>
#include <fstream>
#include <filesystem>

#include "share/Spdz2kShare.h"
#include "offline/FakeOffline.h"
#include "offline/FakeCircuit.h"
#include "MultiplyTruncConfig.h"

int main() {
    auto path = std::filesystem::temp_directory_path();
    std::cout << path << '\n';

    std::array<std::ofstream, 2> files{
            std::ofstream(path / "0.txt"),
            std::ofstream(path / "1.txt")
    };

    FakeOffline<64, 64, 2> offline(files);
    FakeCircuit<Spdz2kShare64, 2> circuit(files, offline);

    auto x = circuit.input(0, 1, 1);
    auto y = circuit.input(0, 1, 1);
    auto a = circuit.multiplyTrunc(x, y);
    auto o = circuit.output(a);

    circuit.addEndpoint(o);
    circuit.runOffline();

    return 0;
}
