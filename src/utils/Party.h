#ifndef MALICIOUS_PPML_PARTY_H
#define MALICIOUS_PPML_PARTY_H


#include <memory>
#include "networking/Network.h"

class Party {
public:
    Party(int myId, int partyNum, int port = 7766)
            : myId(myId), partyNum(partyNum), network(new Network(myId, port)) {
        network->connect();
    }

    Network &getNetwork() const { return *network; }

private:
    int myId;
    int partyNum;
    std::unique_ptr<Network> network;
};


#endif //MALICIOUS_PPML_PARTY_H
