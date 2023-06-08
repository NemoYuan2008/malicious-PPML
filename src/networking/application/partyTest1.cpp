#include <iostream>
#include "share/Spdz2kShare.h"
#include "utils/Party.h"
#include "share/uint128.h"

using namespace std;
int main() {
    auto path = std::filesystem::temp_directory_path();
    Party<Spdz2kShare32> party(1, 2, (path / "1.txt").string());
    __uint128_t x;
    party.getNetwork().rcv(0, &x);
    cout << hex << x;
}