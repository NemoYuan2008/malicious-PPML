#include <cstddef>
#include <iostream>
#include <iomanip>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>


class Network {

public:
    Network(uint32_t id, uint16_t port = 7766, uint16_t secLevel = 128);

    uint32_t getSeclevel() const;

    uint32_t getID() const;

    const char *getAddr();

    bool send(uint party_id, const uint8_t *buf, uint32_t numBytes);

    bool rcv(uint party_id, uint8_t *buf, uint32_t numBytes);

    uint16_t nparties = 2;

    void connect();

    void close(); // close the sockets

private:
    const uint32_t id;
    const uint16_t port;
    const char *addr = "127.0.0.1";
    const uint16_t secLevel;
    std::vector<boost::asio::ip::tcp::socket *> sockets;
    uint64_t counter = 0;
    boost::mutex mutex;
//  bool send(uint party_id,const uint8_t *buf, uint32_t numBytes);
//  bool rcv(uint party_id,const uint8_t *buf, uint32_t numBytes);
};

