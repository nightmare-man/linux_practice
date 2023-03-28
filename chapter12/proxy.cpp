//使用fork出来的子进程来处理时间请求

#include <iostream>
#include <sstream>
#include <sys/wait.h>
#include "tcp.h"
#include <errno.h>
#include <signal.h>
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
        //wait用于释放资源，进程终止或者被杀死，不一定释放了资源， 用来防治僵尸进程zombie
        //wait(nullptr);
        //父进程不等待了，直接继续accept
        //由信号来处理子进程
    }
}
void child_waiter(int sig){
    //这里使用了waitpid()而不是wait wait一次只能处理一个
    //后面的如果同时进入信号处理会被阻塞及遗漏
    //因此使用waitpid-1 表示等待所有子进程，一次等一个，但是使用循环
    //WNOHANG表示如果没有退出的子进程那就立刻返回，防止阻塞
    //waitpid释放进程成功时返回pid

    //通过实验，不加wait处理用ps命令可以看到date进程仍在
    //处于z状态，zombie
    //linux为什么要这么设计呢？因为想让父进程拿到子进程结束时的状态
    //因此没有立马释放子进程所有资源，可以通过wait waitpid中的那个指针，拿到信息
    while(waitpid(-1,nullptr,WNOHANG)>0);
}
int main(int ac,char*av[]){
    if(ac<3) return -1;
    std::istringstream in{av[2]};
    int port=0;
    in>>port;
    tcpserver server{av[1],port};
    server.Handle(handle);
    //signal(SIGCHLD,child_waiter);
    while(true){
        int ret=server.Accept();
        if(ret==-1&&errno!=EINTR) break;
    }
}