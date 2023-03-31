/*
    此程序用于模拟高并发的客户端请求，有以下几点经验
    1 detached属性是attr里设置的 用pthread_attr_setdeatchstate
    而pthread_deatc()是创建了detached属性的线程后调用的，用于表明其
    能自动回收，不能对一个属性为joinable（默认）的线程调用，joinable要
    用join阻塞回收
    2 linux系统能同时打开的文件有限制，因此这个客户端模拟时会报错
    6000request后就再没法拿到新socket——fd了
*/


#include <iostream>
#include <pthread.h>
#include "../chapter12/tcp.h"
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
using namespace tcp;
const char* reqstr="GET /http_threads.cpp HTTP/1.0\r\n";
int len=0;
void* request_work(void* arg){
    tcpclient client{"127.0.0.1",9999};
    client.Write(reqstr,len);
    //不需要手动关闭连接，client析构函数会自己调用
    return nullptr;
}
void handler(int sig){

}
int main(){
    len=strlen(reqstr);
    pthread_t work;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    //pthread_detach是用来执行分离的，比不是说设置为分离属性
    //根据man 线程要么是joinable要么是detached 
    //前者要在创建后执行pthread_join 后者需要执行pthread_detach
    //pthread_detach(work);
    struct itimerval timer;
    timer.it_interval.tv_sec=0;
    timer.it_interval.tv_usec=20*1000;  //500ms
    timer.it_value.tv_sec=0;
    timer.it_value.tv_usec=20*1000;
    signal(SIGALRM,handler);
   // setitimer(ITIMER_REAL,&timer,nullptr);
    
    while(true){
        pthread_create(&work,&attr,request_work,nullptr);
        pthread_detach(work);
       // pause();//等待被计时器唤醒 
        //出现了termineted recursively 因为多线程没来得及输出错误信息，
        //下一线程也报错了，所以就recursively了，这对调试不利，不知道
        //错误到底是什么，因此先慢一点 看看有没有机会得到调试信息。
    }
}