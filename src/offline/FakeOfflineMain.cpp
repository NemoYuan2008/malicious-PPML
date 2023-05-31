#include <iostream>
#include <fstream>
#include "share/Spdz2kShare.h"
#include "offline/FakeOfflineGate.h"

int main() {
    std::ofstream file;
    FakeAdditionGate<Spdz2kShare32> a(file);

    return 0;
}
