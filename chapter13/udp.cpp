#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include "udp.h"
int make_internet_address(const char* addr_str,int port,sockaddr_in*addr){
    addr->sin_family=AF_INET;
    addr->sin_port=htons(port);
    inet_aton(addr_str,&(addr->sin_addr));
    return 0;
}
int make_dgram_server_socket(int port){
    struct sockaddr_in tmp;
    make_internet_address("127.0.0.1",port,&tmp);
    int sock_id=socket(AF_INET,SOCK_DGRAM,0);
    if(bind(sock_id,reinterpret_cast<sockaddr*>(&tmp),sizeof(tmp))==-1)return -1;
    return sock_id;
}
int make_dgram_client_socket(){
    return socket(AF_INET,SOCK_DGRAM,0);
}
int get_internet_address(char* addr_str,int  len,int* port,sockaddr_in* addr){
    char* str=inet_ntoa(addr->sin_addr);
    strncpy(addr_str,str,len);
    *port=int(ntohs(addr->sin_port));
    return 0;
}