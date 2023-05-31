//
// Created by yuan on 23-5-26.
//

#include "uint128.h"


using std::ostream;
using std::istream;
using std::setfill;
using std::setw;
using std::hex;
using std::dec;


ostream &operator<<(ostream &os, __uint128_t x) {
    os << static_cast<uint64_t>(x >> 64) << ' '
       << static_cast<uint64_t>(x);
    return os;
}


istream &operator>>(istream &is, __uint128_t &x) {
    uint64_t upper, lower;
    is >> upper >> lower;
    x = static_cast<__uint128_t>(upper) << 64 | lower;
    return is;
}
