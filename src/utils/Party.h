#ifndef MALICIOUS_PPML_PARTY_H
#define MALICIOUS_PPML_PARTY_H


#include <fstream>
#include <string>
#include <memory>
#include "networking/Network.h"

class Party {
public:
    Party(int myId, int partyNum, const std::string &offlineFilePath, int port = 7766)
            : myId(myId),
              partyNum(partyNum),
              fileStream(offlineFilePath),
              network(myId, port) {
        network.connect();
    }

    Party(const Party &) = delete;

    Party &operator=(const Party &) = delete;

    int getMyId() const { return myId; }

    int getPartyNum() const { return partyNum; }

    std::ofstream &getFileStream() { return fileStream; }

    Network &getNetwork() { return network; }

private:
    const int myId;
    const int partyNum;
    std::ofstream fileStream;
    Network network;
};


#endif //MALICIOUS_PPML_PARTY_H
