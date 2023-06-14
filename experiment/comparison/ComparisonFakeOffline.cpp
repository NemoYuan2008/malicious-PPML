#include <iostream>
#include <fstream>
#include <filesystem>

#include "share/Spdz2kShare.h"
#include "offline/FakeOffline.h"
#include "offline/FakeGate.h"
#include "offline/FakeCircuit.h"

#include "ComparisonConfig.h"

int main() {
    auto path = std::filesystem::temp_directory_path();
    std::cout << path << '\n';

    std::array<std::ofstream, 2> files{
            std::ofstream(path / "0.txt"),
            std::ofstream(path / "1.txt")
    };

    FakeOffline<32, 32, 2> offline(files);
    FakeCircuit<Spdz2kShare32, 2> circuit(files, offline);


    auto x = circuit.input(0, rows, cols);
    auto b = circuit.gtz(x);
    auto o = circuit.output(b);
    circuit.addEndpoint(o);

    circuit.runOffline();

    return 0;
}
