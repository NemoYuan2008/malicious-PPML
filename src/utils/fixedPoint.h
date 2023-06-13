#ifndef MALICIOUS_PPML_FIXEDPOINT_H
#define MALICIOUS_PPML_FIXEDPOINT_H


#include <type_traits>

namespace {
    const int truncateBits = 8;
    const int truncateValue = 1 << truncateBits;
}


template<typename ClearType>
ClearType double2fix(double x) {
    return static_cast<ClearType>(x * truncateValue);
}


template<typename ClearType>
double fix2double(ClearType x) {
    return static_cast<double>(static_cast<std::make_signed_t<ClearType>>(x)) / truncateValue;
}

#endif //MALICIOUS_PPML_FIXEDPOINT_H
