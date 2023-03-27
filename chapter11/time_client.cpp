#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SERVER_ADDR_STR "127.0.0.1"
#define SERVER_PORT 12000
#define BUF_SIZE 128
using namespace std;
int main(){
    int socket_id=socket(AF_INET,SOCK_STREAM,0);
    if(socket_id==-1) throw runtime_error{"can't allocate socket"};
    struct sockaddr_in saddr;
    saddr.sin_port=htons(SERVER_PORT);
    saddr.sin_family=AF_INET;
    inet_aton(SERVER_ADDR_STR,&(saddr.sin_addr));
    int ret=connect(socket_id,reinterpret_cast<sockaddr*>(&saddr),sizeof(saddr));
    if(ret==-1) throw runtime_error{"can't connect"};
    //根据man 9 socket的描述，默认情况下 socket 读写都是阻塞的
    char buf[BUF_SIZE]{0};
    int len=read(socket_id,buf,BUF_SIZE);
    if(len==-1) throw runtime_error{"can't read"};
    cout<<buf<<endl;
    close(socket_id);
}