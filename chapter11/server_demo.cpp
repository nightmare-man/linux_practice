#include <sys/socket.h>
#include <netinet/ip.h>//引入ipv4结构体及相关字节序等函数sockaddr_in
#include <arpa/inet.h>//引入字符串转网络字节序函数
#include <sys/types.h>
#include <iostream>
#include <string>
using namespace std;
#define BIND_ADDR "127.0.0.1"
/*
    socket 创建socket
    bind 绑定本地地址
    listen 开始监听该地址的链接，
*/
int main(){
    //使用ipv4协议族 使用流，因为只有tcp使用流，因此0即代表tcp
    int sock_id=socket(PF_INET,SOCK_STREAM,0);
    if(sock_id==-1) throw runtime_error{"can't open socket"};
    struct sockaddr_in pad; 
    pad.sin_family=AF_INET;
    pad.sin_port=htons(5432);
    inet_aton(BIND_ADDR,&(pad.sin_addr));
    int ret=bind(sock_id,reinterpret_cast<sockaddr*>(&pad),sizeof(sockaddr_in));
    if(ret==-1) cout<<"can;t bind"<<endl;
    listen(sock_id,5);
    socklen_t len=0;
    ret=accept(sock_id,reinterpret_cast<sockaddr*>(&pad),&len) ;  
    cout<<ret<<endl;
    return 0;
}