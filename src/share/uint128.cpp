//
// Created by yuan on 23-5-26.
//

#include <type_traits>
#include <sodium.h>
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


__uint128_t rand128() {
    uint8_t buf[16];
    randombytes_buf(buf, 16);
    return *reinterpret_cast<__uint128_t *>(buf);
}

