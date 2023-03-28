//使用fork出来的子进程来处理时间请求

#include <iostream>
#include <sstream>
#include <sys/wait.h>
#include "tcp.h"
using namespace tcp;
void handle(int fd){
    int pid=fork();
    if(pid==-1) throw std::runtime_error{"fork error"};
    else if(pid==0){
        dup2(fd,1);
        //这里关闭的因该是fd而不是1
        //因为1已经指向了 原来fd指的了
        //关闭文件描述符不等于关闭指向的文件
        //除非所有指向该文件的描述符都关了
        close(fd);
        execl("/usr/bin/date","date",nullptr);
        exit(1);
    }else{
        wait(nullptr);
    }
}
int main(int ac,char*av[]){
    if(ac<3) return -1;
    std::istringstream in{av[2]};
    int port=0;
    in>>port;
    tcpserver server{av[1],port};
    server.Handle(handle);
    server.Accept();
    std::cout<<"end";
}