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
    string ipS = "47.102.107.191";
    uint id = 0;
    uint16_t port = 5000;
    io_context ios;
    auto socket = tcp::socket(ios);

    tcp::endpoint ep(ip::address::from_string(ipS), port);

    ip::tcp::acceptor acceptor(ios, ep);
    acceptor.accept(socket);
    boost::system::error_code ec;
    acceptor.bind(ep,ec);
    //socket.send("hello");
//  for (int i = 0; i < q.size(); ++i) {
//        std::cout<<q[i];
//  }
  std::cout<<"\n";
//  uint8_t * msg;
//  msg = (uint8_t*)malloc(1);
//  cout<< "hello world!"<<endl;
//  player.rcv(1,msg,1);
//  cout<< "receive: ";
//  cout<<msg[0]<<endl;
//  free(msg);
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
