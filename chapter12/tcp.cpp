#include "tcp.h"
#include <iostream>
#include <errno.h>
namespace tcp{
TcpServer::TcpServer(const char* addr_s,int pport):addr_str{addr_s},port{pport},cb{nullptr}{
    socket_id=socket(AF_INET,SOCK_STREAM,0);
    if(socket_id==-1) throw TcpException{"socket error"};
    struct sockaddr_in tmp_ad;
    tmp_ad.sin_family=AF_INET;
    tmp_ad.sin_port=htons(pport);
    inet_aton(addr_s,&(tmp_ad.sin_addr));
    if(bind(socket_id,reinterpret_cast<sockaddr*>(&tmp_ad),sizeof(tmp_ad))==-1){
        close(socket_id);
        throw TcpException{"bind error"};
    } 
    if(listen(socket_id,1)==-1) {
        close(socket_id);
        throw TcpException{"listen error"};
    }
}

int TcpServer::Accept(){
    struct sockaddr_in tmp;
    socklen_t len=0;
    int new_fd=-1;
    new_fd=accept(socket_id,reinterpret_cast<sockaddr*>(&tmp),&len);
    if(new_fd==-1) return -1;
    connected_id.push_back(new_fd);
    if(cb) cb(new_fd);
    return new_fd;
}
int TcpServer::Read(char* buf,int size)const{
    return read(connected_id[connected_id.size()-1],buf,size);
}
int TcpServer::Write(const char* buf,int size)const{
    return write(connected_id[connected_id.size()-1],buf,size);
}
TcpClient::TcpClient(const char* addr_s,int pport):port{pport},addr_str{addr_s}{
    socket_id=socket(AF_INET,SOCK_STREAM,0);
    if(socket_id==-1) throw TcpException{"socket error"};
    struct sockaddr_in tmp;
    tmp.sin_family=AF_INET;
    tmp.sin_port=htons(pport);
    inet_aton(addr_s,&(tmp.sin_addr));
    if(connect(socket_id,reinterpret_cast<sockaddr*>(&tmp),sizeof(tmp))==-1){
        close(socket_id);
        throw TcpException{"connect error"};
    }
}
int TcpClient::Read(char* buf,int size)const{
    return read(socket_id,buf,size);
}
int TcpClient::Write(const char* buf,int size)const{
    return write(socket_id,buf,size);
}
}
