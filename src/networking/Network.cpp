#include <iostream>
#include <iomanip>
#include <cstring>
#include <boost/thread.hpp>
#include "Network.h"

using namespace boost::asio;
using ip::tcp;
using std::cout;
using std::endl;
using std::array;

Network::Network(uint32_t id, uint16_t port)
        : id(id), port(port) {
    //   io_service ios;

//    if (id==1)
//    {
//        tcp::socket *socket = new tcp::socket (ios);
//        cout << "endpoint: "<<port<<endl;
//        socket->connect(tcp::endpoint(ip::address::from_string(addr), port));
//        sockets.push_back(socket);
//        cout<< "connected"<<endl;
//    }
//    else{
//        cout << "endpoint: "<<port<<endl;
//        ip::tcp::acceptor acceptor(ios,tcp::endpoint(ip::address::from_string(addr), port));
//        tcp::socket *socket = new tcp::socket (ios);
//        acceptor.accept(*socket);
//        sockets.push_back(socket);
//        std::cout << socket->remote_endpoint().address() << std::endl;
//    }
//    cout << "initialized"<<endl;

}

void Network::close() {
    for (size_t i = 0; i < sockets.size(); ++i) {
        tcp::socket *socket = sockets.at(i);
        socket->shutdown(socket_base::shutdown_both);
        //socket->close();
    }
}

bool Network::send(uint party_id, const uint8_t *buf, uint32_t numBytes) {

    uint socket_id = party_id;
    if (party_id > sockets.size()) {
        std::cerr << "send failed! server with server id " << party_id << " is not added" << endl;
        return false;
    }
    if (party_id >= id) socket_id--;
    boost::system::error_code err;
    cout << "start sending" << endl;
    write(*sockets[socket_id], buffer(buf, numBytes), err);
    //sockets[socket_id]->write_some(buffer(buf, numBytes), err);
    if (!err) {
        return true;
    } else {
        cout << "send failed: " << err.message() << endl;
        return false;
    }
}

bool Network::send(uint party_id, uint64_t *data) {
    uint socket_id = party_id;
    if (party_id > sockets.size() || party_id == id) {
        std::cerr << "send failed! server id " << party_id << " is not legal" << endl;
        return false;
    }
    if (party_id > id) socket_id--;
    boost::system::error_code err;
    // uint64 --> char*
    uint8_t *buf;
    buf = (uint8_t *) malloc(8);
    memcpy(buf, data, 8);
    write(*sockets[socket_id], buffer(buf, 8), err);
    //sockets[socket_id]->write_some(buffer(buf, numBytes), err);
    free(buf);
    if (!err) {
        return true;
    } else {
        cout << "send failed: " << err.message() << endl;
        return false;
    }
}

bool Network::send(uint party_id, __uint128_t *data) {
    uint socket_id = party_id;
    if (party_id > sockets.size() || party_id == id) {
        std::cerr << "send failed! server id " << party_id << " is not legal" << endl;
        return false;
    }
    if (party_id > id) socket_id--;
    boost::system::error_code err;
    // uint64 --> char*
    uint8_t *buf = (uint8_t *) malloc(16);
    uint8_t *temp;
    memcpy(buf, data, 16);
    write(*sockets[socket_id], buffer(buf, 16), err);
    free(buf);
    //sockets[socket_id]->write_some(buffer(buf, numBytes), err);
    if (!err) {
        return true;
    } else {
        cout << "send failed: " << err.message() << endl;
        return false;
    }
}


bool Network::rcv(uint party_id, uint8_t *buf, uint32_t numBytes) {
    uint socket_id = party_id;
    if (party_id > sockets.size() || party_id == id) {
        std::cerr << "send failed! server id " << party_id << " is not legal" << endl;
        return false;
    }
    if (party_id > id) socket_id--;
    streambuf recBuff;

    boost::system::error_code err;
    read(*sockets[socket_id], recBuff, transfer_at_least(numBytes), err);
    if (err && err != error::eof) {
        std::cerr << "receive failed: " << err.message() << endl;
        return false;
    } else {
        memcpy(buf, buffer_cast<const void *>(recBuff.data()), numBytes);
        return true;
    }
}

bool Network::rcv(uint party_id, uint64_t *data) {
    uint socket_id = party_id;
    if (party_id > sockets.size() || party_id == id) {
        std::cerr << "send failed! server id " << party_id << " is not legal" << endl;
        return false;
    }
    if (party_id > id) socket_id--;
    //streambuf recBuff;
    mutable_buffer recBuff = buffer(data,8); //This is dangerous if a read_err occurred
    boost::system::error_code err;
    read(*sockets[socket_id], recBuff, transfer_at_least(8), err);
    if (err && err != error::eof) {
        std::cerr << "receive failed: " << err.message() << endl;
        return false;
    } else {
        //memcpy(data, buffer_cast<const void *>(recBuff.data()), 8);
        cout << "received. " << endl;
        return true;
    }
}

bool Network::rcv(uint party_id, __uint128_t *data) {
    uint socket_id = party_id;
    if (party_id > sockets.size() || party_id == id) {
        std::cerr << "send failed! server id " << party_id << " is not legal" << endl;
        return false;
    }
    if (party_id > id) socket_id--;
    //streambuf recBuff;
    mutable_buffer recBuff = buffer(data,16); //This is dangerous if a read_err occurred
    boost::system::error_code err;
    read(*sockets[socket_id], recBuff, transfer_at_least(16), err);
    if (err && err != error::eof) {
        std::cerr << "receive failed: " << err.message() << endl;
        return false;
    } else {
        //memcpy(data, buffer_cast<const void *>(recBuff.data()), 16);
        cout << "received." << endl;
        return true;
    }
}


uint32_t Network::getID() const { return this->id; }

const char *Network::getAddr() { return this->addr; }

void Network::connect() {
    io_service ios;
    if (id == 1) {
        tcp::socket *socket = new tcp::socket(ios);
        cout << "endpoint: " << port << endl;
        socket->connect(tcp::endpoint(ip::address::from_string(addr), port));
        sockets.push_back(socket);
        cout << "connected" << endl;
    } else {
        cout << "endpoint: " << port << endl;
        ip::tcp::acceptor acceptor(ios, tcp::endpoint(ip::address::from_string(addr), port));
        tcp::socket *socket = new tcp::socket(ios);
        acceptor.accept(*socket);
        sockets.push_back(socket);
        //std::cout << socket->remote_endpoint().address() << std::endl;
    }

}
