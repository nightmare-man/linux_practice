#include "tcp.h"
#include <sstream>
#include <iostream>
#include <string>
using namespace tcp;
int main(int ac,char*av[]){
    if(ac<3) return -1;
    int port=-1;
    std::istringstream in{av[2]};
    in>>port;
    tcpclient client(av[1],port);
    char buf[128];
    client.Read(buf,128);
    std::cout<<buf;
    return 0;
}