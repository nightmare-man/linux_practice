#include "tcp.h"
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <iostream>
#include <sstream>
#include "http_core.h"
using namespace tcp;

void handler(int fd){
    int pid=fork();
    if(pid==-1) throw std::runtime_error{"fork error"};
    else if(pid==0){
        dup2(fd,0);
        dup2(fd,1);
        close(fd);
        HttpCore core{};
        core.Exec();
        exit(1);
    }else{
        //父进程直接返回
    }
    close(fd);
}
void child_handler(int sig){
    while(waitpid(-1,nullptr,WNOHANG)>0);
}
int main(int ac,char*av[]){
    if(ac<2)return -1;
    std::istringstream in{av[1]};
    int port=0;
    in>>port;
    tcpserver server{"172.22.13.190",port};
    server.Handle(handler);
    signal(SIGCHLD,child_handler);
    while(true){
        int ret=server.Accept();
        if(ret==-1){
            if(errno==EINTR) continue;//被处理SIGCHLD打断返回-1的
            throw std::runtime_error{"accept error"};
        }
    }
}