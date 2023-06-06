#include <iostream>
#include "utils/Party.h"
#include "share/uint128.h"

using namespace std;
int main() {
    Party party(1, 2, 7767);
    __uint128_t x;
    party.getNetwork().rcv(0, &x);
    cout << hex << x;
}