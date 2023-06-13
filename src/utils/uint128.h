#ifndef MALICIOUS_PPML_UINT128_H
#define MALICIOUS_PPML_UINT128_H

#include <iostream>
#include <iomanip>
#include <cstdint>

inline
std::ostream &operator<<(std::ostream &os, __uint128_t x) {
    os << static_cast<uint64_t>(x >> 64) << ' '
       << static_cast<uint64_t>(x);
    return os;
}

inline
std::istream &operator>>(std::istream &is, __uint128_t &x) {
    uint64_t upper, lower;
    is >> upper >> lower;
    x = static_cast<__uint128_t>(upper) << 64 | lower;
    return is;
}

inline
std::ostream &operator<<(std::ostream &os, __int128_t x) {
    os << static_cast<int64_t>(x >> 64) << ' '
       << static_cast<int64_t>(x);
    return os;
}

inline
std::istream &operator>>(std::istream &is, __int128_t &x) {
    int64_t upper, lower;
    is >> upper >> lower;
    x = static_cast<__int128_t>(upper) << 64 | lower;
    return is;
}


#endif //MALICIOUS_PPML_UINT128_H
