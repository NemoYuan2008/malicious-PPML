#include <iostream>
#include <fstream>
#include "share/Spdz2kShare.h"
#include "offline/FakeOfflineGate.h"

int main() {
    std::ofstream file;
    FakeAdditionGate<Spdz2kShare32> a(file);

    std::cin >> std::hex;
    std::cout << std::hex;

    Spdz2kShare32 s;
    std::cin >> s;
    std::cout << s << '\n';

    Spdz2kShare64 s2;
    std::cin >> s2;
    std::cout << s2 << '\n';

    return 0;
}
