#include "tcp.h"
#include <sstream>
#include <string>
#include <iostream>
using namespace tcp;
int main(int ac,char* av[]){
    if(ac<2) return -1;
    std::istringstream in{av[1]};
    int port=0;
    in>>port;
    std::cout<<port<<std::endl;
    tcpserver server{"172.22.13.190",port};
    server.Accept();
    const char* a="hello";
    server.Write(a,6);
    return 0;
}