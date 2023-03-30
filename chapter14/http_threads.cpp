
#include <iostream>
#include <sstream>
#include <pthread.h>
#include <string.h>
#include "../chapter12/tcp.h"
#include "../chapter12/http_core.h"
#include <signal.h>
using namespace tcp;
//这种设置防止同一线程连续lock进而死锁
pthread_mutex_t mutex=PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;
unsigned long long byte_cnt=0;
void* work_routine(void* arg){
    int fd=*(reinterpret_cast<int*>(arg));
    char buf[1024]{0};
    read(fd,buf,1024);
    HttpCore core{buf};
    core.Exec(buf,1024);
    int len=strlen(buf);
    pthread_mutex_lock(&mutex);
    byte_cnt+=(len);
    pthread_mutex_unlock(&mutex);
    write(fd,buf,len);
    close(fd);
    return nullptr;
}
//信号处理 有些针对进程的，有些针对线程的，SIGINT
//就是针对进程的，会随机给到能够没有屏蔽该信号的线程，
//而linux的互斥锁，默认情况下不能同一个进程连续锁，会导致自己死锁
//因此，要么改变这个属性，要么就别试图在信号处理里拿锁。
void sig_handler(int sig){
    pthread_mutex_lock(&mutex);
    std::cout<<"Total bytes send now : "<<byte_cnt<<std::endl;
    pthread_mutex_unlock(&mutex);
}
int main(int ac,char*av[]){
    const char* ip_str="127.0.0.1";
    int port=9999;
    switch (ac)
    {
    case 1:
        break;
    case 2:
        {
            std::istringstream in{av[1]};
            in>>port;
        }
        break;
    default:
        {
            ip_str=const_cast<const char*>(av[1]);
            std::istringstream in{av[1]};
            in>>port;
        }
        break;
    }
    tcpserver server{ip_str,port};
    pthread_t work;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    //根据man得知：
    //设置线程为detached（独立的） 非独立的线程需要用join回收资源
    //也可以用pthread_detach()设置
    signal(SIGINT,sig_handler);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);   
    while (true)
    {
        int fd=server.Accept();
        pthread_create(&work,&attr,work_routine,reinterpret_cast<void*>(&fd));    
    }
    
}