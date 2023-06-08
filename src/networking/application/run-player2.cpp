//
// Created by ysx on 6/5/23.
//
#include <iostream>
#include <cstddef>
#include <boost/asio.hpp>
//#include <boost/bind.hpp>
//#include <boost/enable_shared_from_this.hpp>
#include <iomanip>
#include "networking/Network.h"

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;
using std::array;


int main (int argc, char **argv) {
    string ipS = "127.0.0.1";
    uint16_t portS = 7766;
    uint16_t port = 7200;
    uint id = 2;
    auto SECURITYLEVEL = 128;

    // for(int i=1; i< argc; i++){
    //   cout<< atoi(argv[i])<<endl;
    // }
    Network player(id,port);
    cout << "initialized"<<endl;
    player.connect();

    //player.close();
    //string filename = "otp_preprocess.db";

    //Create Network, connect sockets

    //Run the offline/online process

    //Player server (id, filename, port, SECURITYLEVEL, ipS, portS);
    // input layers
    // testFunc(&server);
    // cout<<"end testing"<<endl;
    return 0;
}
