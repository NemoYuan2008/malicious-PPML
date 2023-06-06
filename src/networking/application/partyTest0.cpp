#include "utils/Party.h"

int main() {
    Party party(0, 2);
    __uint128_t x = static_cast<__uint128_t>(0x12345678abcdef0) << 64 | 0x0fedbcda87654321;
    party.getNetwork().send(1, &x);
}
