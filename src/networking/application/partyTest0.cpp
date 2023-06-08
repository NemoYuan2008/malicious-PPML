#include <iostream>
#include "share/Spdz2kShare.h"
#include "utils/Party.h"
#include "share/uint128.h"


int main() {
    auto path = std::filesystem::temp_directory_path();
    Party<Spdz2kShare32> party(0, 2, (path / "0.txt").string());
    __uint128_t x = static_cast<__uint128_t>(0x12345678abcdef0) << 64 | 0x0fedbcda87654321;
    party.getNetwork().send(1, &x);
}
