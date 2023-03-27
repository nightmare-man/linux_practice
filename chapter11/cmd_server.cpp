/*
    建立连接
    /popen原理 pipe fork exec /
    popen一个进程并获取输出结果
    发送给客户端
*/
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#define HOST_IP "127.0.0.1"
#define HOST_PORT 7400
using namespace std;
void error(const char* msg){
    throw runtime_error{msg};
}
void handle_cmd(int fd){
    stringstream buf{};
    buf<<"server connected,type cmd here:"<<endl;
    //write发往内核缓冲区就返回，不一定真的就发出去了
    
    write(fd,buf.rdbuf()->str().c_str(),buf.rdbuf()->str().size());
    while(true){
        char buf[128]{0};
        //read在内核缓冲区已有数据就返回，不一定能读到要求的字节数
        read(fd,buf,128);
        for(int i=0;i<strlen(buf);i++) if(buf[i]=='\r') buf[i]=0;
        if(strcmp(buf,"exit")==0){
            cout<<"handler end"<<endl;
            break;
        }else{
            cout<<buf;
            for(int i=0;i<strlen(buf);i++) cout<<buf[i]<<endl;
            FILE* p=popen(buf,"r");
            while(fgets(buf,128,p)!=nullptr){
                cout<<buf;//会存入\n
            }
        }
    }
}
int main(){
    sockaddr_in add;
    add.sin_family=AF_INET;
    add.sin_port=htons(HOST_PORT);
    inet_aton(HOST_IP,&(add.sin_addr));
    int sock_id=socket(AF_INET,SOCK_STREAM,0);
    if(sock_id==-1) error("socket error");
    if(bind(sock_id,reinterpret_cast<sockaddr*>(&add),sizeof(add))==-1) error("bind error") ;
    if(listen(sock_id,1)==-1) error("listen error");
    while(true){
        socklen_t len=0;
        int new_fd=accept(sock_id,reinterpret_cast<sockaddr*>(&add),&len);
        if(new_fd==-1) error("accept error");
        cout<<"[new connection]"<<inet_ntoa(add.sin_addr)<<endl;
        handle_cmd(new_fd);
        close(new_fd);
    }
}