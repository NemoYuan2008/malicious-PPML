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
    sockets.resize(nparties-1);
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
    cout << "start sending\n";
    write(*sockets[socket_id], buffer(buf, numBytes), err);
    //sockets[socket_id]->write_some(buffer(buf, numBytes), err);
    if (!err) {
        return true;
    } else {
        std::cout << "send failed: " << err.message()<<'\n';
        return false;
    }
}

bool Network::send(uint party_id, uint64_t *data) {
    uint socket_id = party_id;
    if (party_id > sockets.size() || party_id == id) {
        std::cerr << "send failed! server id " << party_id << " is not legal.\n";
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
        std::cout << "send failed: " << err.message() << '\n';
        return false;
    }
}

bool Network::send(uint party_id, __uint128_t *data) {
    uint socket_id = party_id;
    if (party_id > sockets.size() || party_id == id) {
        std::cerr << "send failed! server id " << party_id << " is illegal.\n";
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
    //2 party only

//    io_service ios;
//    if (id == 1) {
//        tcp::socket *socket = new tcp::socket(ios);
//        cout << "endpoint: " << port << endl;
//        socket->connect(tcp::endpoint(ip::address::from_string(addr), port));
//        sockets[0] = socket;
//        cout << "connected" << endl;
//    } else {
//        cout << "endpoint: " << port << endl;
//        ip::tcp::acceptor acceptor(ios, tcp::endpoint(ip::address::from_string(addr), port));
//        tcp::socket *socket = new tcp::socket(ios);
//        acceptor.accept(*socket);
//        sockets[0]=socket;
//        //std::cout << socket->remote_endpoint().address() << std::endl;
//    }

    //multi-party using threads


    for (int i = 0; i < id; ++i) {
        io_service ios;
        tcp::socket *socket = new tcp::socket(ios);
        std::cout<< "connect to player "<< i <<", port: " << port + i*10 + id <<"\n";
        socket->connect(tcp::endpoint(ip::address::from_string(addr), port + i * 10 + id));
        sockets[i] = socket;
        std::cout<<"player "<<i<<" connected.\n";
    }

    std::vector<boost::thread> initThreads;
    for (uint32_t i = id+1; i < nparties; i++) {
       initThreads.push_back(boost::thread (connect2player, i, this));
    }
    for(uint32_t i = 0; i < nparties-id-1; ++i) {
        initThreads[i].join();
    }
    initThreads.clear();
    std::cout<<"connected\n";
}

void connect2player(uint8_t partyid, Network* player) {
    player->ConnectTo(partyid);
}

void Network::ConnectTo(uint32_t partyid) { //TODO: should read a local address table: partyid - addr - port
    if (partyid == id || partyid > nparties-1){
        printmutex.lock();
        cout << "partyid is illegal.\n";
        printmutex.unlock();
        return;
    }
    auto socketid = partyid;
    if (partyid > id) socketid--;
    io_service ios;
    tcp::socket *socket = new tcp::socket(ios);
    if (partyid < id) {
        socket->connect(tcp::endpoint(ip::address::from_string(addr), port + partyid * 10 + id));
        mutex.lock();
        sockets[socketid] = socket;
        mutex.unlock();
    }
    else{
        printmutex.lock();
        std::cout<< "waiting for player "<< partyid<<", port: "<<port+id*10 + partyid<<"\n";
        printmutex.unlock();
        ip::tcp::acceptor acceptor(ios,tcp::endpoint(ip::address::from_string(addr), port+id*10 + partyid));
        acceptor.accept(*socket);
        mutex.lock();
        sockets[socketid] = socket;
        mutex.unlock();
        //std::cout << socket->remote_endpoint().address() << std::endl;
    }
#ifdef STATE
    printmutex.lock();
    std::cout<<"connected to player "<<partyid<<"\n";
    printmutex.unlock();
#endif
}