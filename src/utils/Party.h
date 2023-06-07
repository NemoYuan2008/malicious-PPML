#ifndef MALICIOUS_PPML_PARTY_H
#define MALICIOUS_PPML_PARTY_H


#include <fstream>
#include <string>
#include <memory>
#include <type_traits>
#include "networking/Network.h"


template<typename ShrType>
class Party {
public:

    Party(int myId, int partyNum, const std::string &offlineFilePath, std::false_type)
        :myId(myId),
         partyNum(partyNum),
         fileStream(offlineFilePath),
         network(myId) {        //No network connection, used only for testing
        fileStream >> partyKey;
    }

    Party(int myId, int partyNum, const std::string &offlineFilePath, int port = 7766)
            : myId(myId),
              partyNum(partyNum),
              fileStream(offlineFilePath),
              network(myId, port) {
        network.connect();
        fileStream >> partyKey;
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
    typename ShrType::PartyKeyType partyKey;
};


#endif //MALICIOUS_PPML_PARTY_H
