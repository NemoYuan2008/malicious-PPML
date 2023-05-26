//
// Created by yuan on 23-5-26.
//

#include "uint128.h"

using std::ostream;
using std::setfill;
using std::setw;
using std::hex;
using std::dec;


ostream &operator<<(ostream &os, __uint128_t x) {
    os << hex
       << setfill('0') << setw(16) << static_cast<uint64_t>(x >> 64)
       << setfill('0') << setw(16) << static_cast<uint64_t>(x)
       << dec;
    return os;
}
