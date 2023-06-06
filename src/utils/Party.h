#ifndef MALICIOUS_PPML_PARTY_H
#define MALICIOUS_PPML_PARTY_H


#include <memory>
#include "networking/Network.h"

class Party {
public:
    Party(int myId, int partyNum, int port = 7766)
            : myId(myId), partyNum(partyNum), network(myId, port) {
        network.connect();
    }

    Party(const Party &) = delete;

    Party &operator=(const Party &) = delete;

    Network &getNetwork() { return network; }

private:
    int myId;
    int partyNum;
    Network network;
};


#endif //MALICIOUS_PPML_PARTY_H
