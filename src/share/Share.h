#ifndef MALICIOUS_PPML_SHARE_H
#define MALICIOUS_PPML_SHARE_H

#include <cstdint>
#include "share/Spdz2kShare.h"


class Share {
private:
    Spdz2kShare sharedDelta;
    std::int64_t openedDelta;
};


#endif //MALICIOUS_PPML_SHARE_H
