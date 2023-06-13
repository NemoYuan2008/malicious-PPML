#ifndef MALICIOUS_PPML_NETWORK_H
#define MALICIOUS_PPML_NETWORK_H
#define STATE

#include <cstddef>
#include <iostream>
#include <iomanip>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#define partyoffset 10
#define socketsoffset 5

class Network {

public:
    Network(uint32_t id, uint16_t port = 7766);

    uint32_t getID() const;

    const char *getAddr();

    bool send(uint party_id, const uint8_t *buf, uint32_t numBytes);

    bool send(uint party_id, uint64_t *data);

    bool send(uint party_id, __uint128_t *data);

//    template<typename T>
//    bool send(uint party_id, T *data);

    template<typename T>
    bool send(uint party_id, const T &data);

    template<typename T>
    bool send(uint party_id, const std::vector<T> &data);

    bool rcv(uint party_id, uint8_t *buf, uint32_t numBytes);

    bool rcv(uint party_id, uint64_t *data);

    bool rcv(uint party_id, __uint128_t *data);

    template<typename T>
    bool rcv(uint party_id, T *data);

    template<typename T>
    bool rcv(uint party_id, std::vector<T> *data, uint32_t numElements);

    uint16_t nparties = 2;

    void connect();

    void ConnectTo(uint32_t partyid, uint type);

    void close(); // close the sockets

private:
    const uint32_t id;
    const uint16_t port;
    const char *addr = "127.0.0.1";
//    std::vector<boost::asio::ip::tcp::socket *> sockets;
    std::vector<boost::asio::ip::tcp::socket *> sendsockets;
    std::vector<boost::asio::ip::tcp::socket *> rcvsockets;
    uint64_t counter = 0;
    boost::mutex rcvmutex, sendmutex;
    boost::mutex printmutex;
//  bool send(uint party_id,const uint8_t *buf, uint32_t numBytes);
//  bool rcv(uint party_id,const uint8_t *buf, uint32_t numBytes);
};


//template<typename T>
//bool Network::send(uint party_id, T *data) {
//    uint socket_id = party_id;
//    if (party_id > sockets.size() || party_id == id) {
//        std::cerr << "send failed! server id " << party_id << " is not legal\n";
//        return false;
//    }
//    if (party_id > id) socket_id--;
//    boost::system::error_code err;
//    // uint64 --> char*
//    uint8_t *buf = (uint8_t *) malloc(sizeof(T));
//    uint8_t *temp;
//    memcpy(buf, data, sizeof(T));
//    write(*sockets[socket_id], boost::asio::buffer(buf, sizeof(T)), err);
//    free(buf);
//    //sockets[socket_id]->write_some(buffer(buf, numBytes), err);
//    if (!err) {
//        return true;
//    } else {
//        std::cout << "send failed: " << err.message() << '\n';
//        return false;
//    }
//}

template<typename T>
bool Network::send(uint party_id, const T &data) {
    uint socket_id = party_id;
    if (party_id > sendsockets.size() || party_id == id) {
        std::cerr << "send failed! server id " << party_id << " is not legal\n";
        return false;
    }
    if (party_id > id) socket_id--;
    boost::system::error_code err;
    // uint64 --> char*
    uint8_t *buf = (uint8_t *) malloc(sizeof(T));
    uint8_t *temp;
    memcpy(buf, &data, sizeof(T));
    write(*sendsockets[socket_id], boost::asio::buffer(buf, sizeof(T)), err);
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
bool Network::send(uint party_id, const std::vector<T> &data) {
    uint socket_id = party_id;
    if (party_id > sendsockets.size() || party_id == id) {
        std::cerr << "send failed! server id " << party_id << " is not legal\n";
        return false;
    }
    if (party_id > id) socket_id--;
    boost::system::error_code err;
    // uint64 --> char*
    uint32_t numBytes;
    numBytes = sizeof(T) * data.size();
    if (numBytes == 0) {
        std::cout << "the vector is empty!\n";
        return false;
    }
//    uint8_t *buf = (uint8_t *) malloc(numBytes);
//    memcpy(buf, data.data(), numBytes);
    uint32_t bufferSize = 2048;
    uint16_t rcvbytes = 0;
    uint16_t sendbytes = 2048;
    auto *dest = data.data();
    write(*sendsockets[socket_id], boost::asio::buffer(dest, numBytes), err);
    if (!err) {
//            dest += bufferSize;
//            numBytes -= bufferSize;
//        std::cout<<numBytes<<" Bytes \n";
    } else {
        std::cout << "send failed: " << err.message() << '\n';
        return false;
    }
//    while (numBytes > sendbytes){
//        sendsockets[socket_id]->write_some(boost::asio::buffer(&sendbytes,2));
//        sendsockets[socket_id]->write_some(boost::asio::buffer(&dest,sendbytes));
//        //write(*sendsockets[socket_id], boost::asio::buffer(dest, sendbytes), err);

//        sendsockets[socket_id]->read_some(boost::asio::buffer(&rcvbytes,2));
//        dest += rcvbytes;
//        sendbytes = rcvbytes < bufferSize ? rcvbytes : bufferSize;
//        numBytes -= rcvbytes;
//    }
//    if(numBytes>0){
//        sendbytes = numBytes;
//        sendsockets[socket_id]->write_some(boost::asio::buffer(&sendbytes,2));
//        sendsockets[socket_id]->write_some(boost::asio::buffer(dest,sendbytes));
//        sendsockets[socket_id]->read_some(boost::asio::buffer(&rcvbytes,2));
//        numBytes -= rcvbytes;
////        write(*sendsockets[socket_id], boost::asio::buffer(dest, sendbytes), err);
////        if (!err) {
//////            dest += bufferSize;
//////            numBytes -= bufferSize;
////        } else {
////            std::cout << "send failed: " << err.message() << '\n';
////            return false;
////        }
//    }
//    if (numBytes!= 0) {
//        std::cout<<"numBytes: "<<numBytes<<'\n';
//    }
    return true;
//    write(*sockets[socket_id], boost::asio::buffer(buf, numBytes), err);
//    free(buf);
    //sockets[socket_id]->write_some(buffer(buf, numBytes), err);
}

template<typename T>
bool Network::rcv(uint party_id, T *data) {
    uint socket_id = party_id;
    if (party_id > rcvsockets.size() || party_id == id) {
        std::cerr << "send failed! server id " << party_id << " is not legal" << '\n';
        return false;
    }
    if (party_id > id) socket_id--;
    //boost::asio::streambuf recBuff;
    boost::asio::mutable_buffer recBuff = boost::asio::buffer(data,
                                                              sizeof(T)); //This is dangerous if a read_err occurred
    boost::system::error_code err;
    read(*rcvsockets[socket_id], recBuff,
         boost::asio::transfer_at_least(sizeof(T)), err);
    if (err && err != boost::asio::error::eof) {
        std::cerr << "receive failed: " << err.message() << '\n';
        return false;
    } else {
        //memcpy(data, boost::asio::buffer_cast<const void *>(recBuff.data()), sizeof(T));
//        std::cout << "received.\n";
        return true;
    }
}

template<typename T>
bool Network::rcv(uint party_id, std::vector<T> *data, uint32_t numElements) {
    uint socket_id = party_id;
    if (party_id > rcvsockets.size() || party_id == id) {
        std::cerr << "send failed! server id " << party_id << " is not legal" << '\n';
        return false;
    }
    if (party_id > id) socket_id--;
    //boost::asio::streambuf recBuff;
    if (data->size() != numElements) {
        std::cout << "vector size doesn't fit.\n";
        return false;
    }
    uint32_t numBytes = numElements * sizeof(T);
//    uint32_t bufferSize = 2048;
//    uint16_t sendbytes = 0;
//    uint16_t rcvbytes;
    boost::system::error_code err;
    auto* dest = data->data();
    read(*rcvsockets[socket_id],boost::asio::mutable_buffer(dest,numBytes),err);
    if (err && err != boost::asio::error::eof) {
        std::cerr << "receive failed: " << err.message() << '\n';
        return false;
    } else {
//        std::cout<<numBytes<<" Bytes \n";
    }
//    while (numBytes > 0){
//        rcvsockets[socket_id]->read_some(boost::asio::buffer(&sendbytes,2));
//        rcvbytes = rcvsockets[socket_id]->read_some(boost::asio::buffer(dest,sendbytes));
//        rcvsockets[socket_id]->write_some(boost::asio::buffer(&rcvbytes,2));
//        dest += rcvbytes;
//        numBytes -= rcvbytes;
////        read(*rcvsockets[socket_id], boost::asio::mutable_buffer(dest,rcvbytes),
////             boost::asio::transfer_at_least(rcvbytes), err);
//
//    }
//    if (numBytes > 0){
//        read(*rcvsockets[socket_id], boost::asio::mutable_buffer(dest,numBytes), boost::asio::transfer_at_least(numBytes), err);
//        if (err && err != boost::asio::error::eof) {
//            std::cerr << "receive failed: " << err.message() << '\n';
//            return false;
//        }
//    }
    return true;
    //uint8_t* buf = (uint8_t*)malloc(numBytes);
    //boost::asio::streambuf recBuff;
    // boost::asio::mutable_buffer recBuff = boost::asio::buffer(data->data(),numBytes); //This is dangerous if a read_err occurred

//    read(*sockets[socket_id], boost::asio::mutable_buffer(), boost::asio::transfer_at_least(numBytes), err);
//    if (err && err != boost::asio::error::eof) {
//        std::cerr << "receive failed: " << err.message() << '\n';
//        return false;
//    } else {
//        //memcpy(data->data(), boost::asio::buffer_cast<const void *>(recBuff.data()), numBytes);
//        std::cout << "received.\n";
//        //free(buf);
//        return true;
//    }
}

void connect2player(uint8_t partyid, Network *player, uint type); //for threads, type=1 make sendsocket acceptor
#endif //MALICIOUS_PPML_NETWORK_H