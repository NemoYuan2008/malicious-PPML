#include <iostream>
#include <iomanip>
#include <cstring>
#include <boost/thread.hpp>
#include "network.h"

using namespace boost::asio;
using ip::tcp;
using std::cout;
using std::endl;
using std::array;

Network::Network(uint32_t id, uint16_t port, uint16_t secLevel)
        : id(id), port(port), secLevel(secLevel) {
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
    write(*sockets[socket_id], buffer(buf, numBytes), err);
    //sockets[socket_id]->write_some(buffer(buf, numBytes), err);
    if (!err) {
        return true;
    } else {
        cout << "send failed: " << err.message() << endl;
        return false;
    }
}

bool Network::rcv(uint party_id, uint8_t *buf, uint32_t numBytes) {
    if (party_id > sockets.size()) {
        std::cerr << "receive failed! server with player id " << party_id << " is not added" << endl;
        return false;
    }
    uint socket_id = party_id;
    if (party_id >= id) socket_id--;
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

uint32_t Network::getSeclevel() const { return this->secLevel; }

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
