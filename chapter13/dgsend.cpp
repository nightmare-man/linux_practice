#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include "udp.h"
int main(){
    struct sockaddr_in tmp;
    make_internet_address("127.0.0.1",9003,&tmp);
    int sock=make_dgram_client_socket();
    const char* buf="nihao\n";
    sendto(sock,buf,7,0,reinterpret_cast<sockaddr*>(&tmp),sizeof(tmp));
}