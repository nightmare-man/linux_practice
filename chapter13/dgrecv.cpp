#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include "udp.h"
void say_who_called(sockaddr_in* addr){
    char ipstr[30]{0};
    int port=0;
    get_internet_address(ipstr,30,&port,addr);
    std::cout<<"["<<ipstr<<","<<port<<"]\n";
    return;
}
int main(){
    int sock=make_dgram_server_socket(9003);
    char buf[128];
    struct sockaddr_in tmp;
    socklen_t len=0;
    while( recvfrom(sock,buf,128,0,reinterpret_cast<sockaddr*>(&tmp),&len) ){
        say_who_called(&tmp);
        std::cout<<buf<<std::endl;
    }
}