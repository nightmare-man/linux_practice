
#include <iostream>
#include <sstream>
#include <pthread.h>
#include <string.h>
#include "../chapter12/tcp.h"
#include "../chapter12/http_core.h"
using namespace tcp;
void* work_routine(void* arg){
    int fd=*(reinterpret_cast<int*>(arg));
    char buf[1024]{0};
    read(fd,buf,1024);
    HttpCore core{buf};
    core.Exec(buf,1024);
    write(fd,buf,strlen(buf));
    close(fd);
    return nullptr;
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
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);   
    while (true)
    {
        int fd=server.Accept();
        pthread_create(&work,&attr,work_routine,reinterpret_cast<void*>(&fd));    
    }
    
}