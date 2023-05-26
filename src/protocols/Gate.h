//
// Created by 袁博实 on 2023/5/26.
//

#ifndef MALICIOUS_PPML_GATE_H
#define MALICIOUS_PPML_GATE_H


#include "protocols/GateOffline.h"

template<int K, int S>
class Gate {
public:
    GateOffline<K, S> *offline;

    virtual void runOffline() {
        offline->runOffline();
    }
};


#endif //MALICIOUS_PPML_GATE_H
