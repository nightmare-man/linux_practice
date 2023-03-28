#include "tcp.h"
#include <sstream>
#include <string>
#include <iostream>
using namespace tcp;
int main(int ac,char* av[]){
    if(ac<3) return -1;
    std::istringstream in{av[2]};
    int port=0;
    in>>port;
    tcpserver server{av[1],port};
    server.Accept();
    const char* a="hello";
    server.Write(a,6);
    return 0;
}