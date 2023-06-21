#include <iostream>
#include <filesystem>
#include <vector>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/Party.h"
#include "utils/ioHelper.h"
#include "ArgmaxConfig.h"

using std::cout;

int main() {
    auto path = std::filesystem::temp_directory_path();

    Party<Spdz2kShare32> party(0, 2, (path / "0.txt").string());
    Circuit<Spdz2kShare32> circuit(&party);

    auto x = circuit.input(0, rows, cols);
    auto b = circuit.argmax(x);
    auto o = circuit.output(b);
    circuit.addEndpoint(o);

    circuit.readOfflineFromFile();

    std::vector<Spdz2kShare32::ClearType> xIn = {0,2,32,40,100,1,2,3,static_cast<Spdz2kShare32::ClearType>(45),0,
                                                 10,2,3,24,5,30,5,Spdz2kShare32::ClearType(433.1),5,100};
    x->setInput(xIn);

    circuit.runOnline();
    printVector(o->getClear());

    return 0;
}
