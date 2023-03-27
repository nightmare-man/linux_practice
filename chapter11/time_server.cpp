#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <time.h>

#define PORTNUM 12000
#define IP_ADDR_STR "127.0.0.1"
using namespace std;
int main(){
    struct sockaddr_in saddr;
    int socket_id=socket(PF_INET,SOCK_STREAM,0);
    if(socket_id==-1) throw runtime_error{"can't allocate socket"};
    saddr.sin_port=htons(PORTNUM);
    saddr.sin_family=AF_INET;
    inet_aton(IP_ADDR_STR,&(saddr.sin_addr));
    if(0!=bind(socket_id,reinterpret_cast<sockaddr*>(&saddr),sizeof(saddr))){
        throw runtime_error{"can't bind addr"};
    }
    if(listen(socket_id,1)!=0) throw runtime_error{"can't listen"};
    stringstream outbuf{};
    
    while(true){
        socklen_t len=0;
        int new_sock_id=accept(socket_id,reinterpret_cast<sockaddr*>(&saddr),&len);
        if(new_sock_id==-1) throw runtime_error{"bad socket"};
        cout<<"connection : "<<inet_ntoa(saddr.sin_addr)<<endl;
        time_t t=time(nullptr);
        outbuf<<"The time here is : "<<ctime(&t)<<endl;
        string strbuf=outbuf.rdbuf()->str();
        write(new_sock_id,strbuf.c_str(),strbuf.size());
        close(new_sock_id);
        //将指针重新回到开头，覆盖之前的内容
        outbuf.seekp(ios::beg);
    }
}