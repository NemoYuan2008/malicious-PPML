#include <iostream>
#include <filesystem>
#include <vector>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/Party.h"
#include "utils/ioHelper.h"
#include "ComparisonConfig.h"

using std::cout;

int main() {
    auto path = std::filesystem::temp_directory_path();

    Party<Spdz2kShare32> party(1, 2, (path / "1.txt").string());
    Circuit<Spdz2kShare32> circuit(&party);

    auto x = circuit.input(0, rows, cols);
    auto b = circuit.gtz(x);
    auto o = circuit.output(b);
    circuit.addEndpoint(o);

    circuit.readOfflineFromFile();

    circuit.runOnline();

    return 0;
}
