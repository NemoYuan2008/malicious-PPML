#include <cstddef>
#include <iostream>
#include <iomanip>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>


class Network {

public:
    Network(uint32_t id, uint16_t port = 7766);

    uint32_t getID() const;

    const char *getAddr();

    bool send(uint party_id, const uint8_t *buf, uint32_t numBytes);

    bool send(uint party_id, uint64_t *data);

    bool send(uint party_id, __uint128_t *data);

    template<typename T>
    bool send(uint party_id, T *data);

    bool rcv(uint party_id, uint8_t *buf, uint32_t numBytes);

    bool rcv(uint party_id, uint64_t *data);

    bool rcv(uint party_id, __uint128_t *data);

    template<typename T>
    bool rcv(uint party_id, T *data);

    uint16_t nparties = 2;

    void connect();

    void close(); // close the sockets

private:
    const uint32_t id;
    const uint16_t port;
    const char *addr = "127.0.0.1";
    std::vector<boost::asio::ip::tcp::socket *> sockets;
    uint64_t counter = 0;
    boost::mutex mutex;
//  bool send(uint party_id,const uint8_t *buf, uint32_t numBytes);
//  bool rcv(uint party_id,const uint8_t *buf, uint32_t numBytes);
};


template<typename T>
bool Network::send(uint party_id, T *data) {
    uint socket_id = party_id;
    if (party_id > sockets.size() || party_id == id) {
        std::cerr << "send failed! server id " << party_id << " is not legal\n";
        return false;
    }
    if (party_id > id) socket_id--;
    boost::system::error_code err;
    // uint64 --> char*
    uint8_t *buf = (uint8_t *) malloc(sizeof(T));
    uint8_t *temp;
    memcpy(buf, data, sizeof(T));
    write(*sockets[socket_id], boost::asio::buffer(buf, sizeof(T)), err);
    free(buf);
    //sockets[socket_id]->write_some(buffer(buf, numBytes), err);
    if (!err) {
        return true;
    } else {
        std::cout << "send failed: " << err.message() << '\n';
        return false;
    }
}


template<typename T>
bool Network::rcv(uint party_id, T *data) {
    uint socket_id = party_id;
    if (party_id > sockets.size() || party_id == id) {
        std::cerr << "send failed! server id " << party_id << " is not legal" << '\n';
        return false;
    }
    if (party_id > id) socket_id--;
    boost::asio::streambuf recBuff;
    boost::system::error_code err;
    read(*sockets[socket_id], recBuff, boost::asio::transfer_at_least(sizeof(T)), err);
    if (err && err != boost::asio::error::eof) {
        std::cerr << "receive failed: " << err.message() << '\n';
        return false;
    } else {
        memcpy(data, boost::asio::buffer_cast<const void *>(recBuff.data()), sizeof(T));
        std::cout << "received.\n";
        return true;
    }
}

