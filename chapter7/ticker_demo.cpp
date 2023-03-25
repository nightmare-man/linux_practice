#include <sys/time.h>
#include <signal.h>
#include <iostream>
using namespace std;
void f(int p){
    static int count=10;
    cout<<"take medicine\n";
    cout.flush();
    if(count--==0){
        cout<<"done\n";
        cout.flush();
        exit(1);
    }
}
//操作系统为每个进程提供三个us级计时器，
//但实际上并没有那么多硬件计时器，而是每隔1us为所有进程计数器减一并检查是否为0
void set_time_interval(int ms){
    int sec=ms/1000;
    int msec=(ms%1000)*1000L;
    itimerval newtimer;
    //设置间隔
    newtimer.it_interval.tv_sec=sec;
    newtimer.it_interval.tv_usec=msec;//这里是微秒不是毫秒
    //设置第一次的初始值（什么时候第一次发送alarm信号）
    newtimer.it_value.tv_sec=sec;
    newtimer.it_value.tv_usec=msec;
    setitimer(ITIMER_REAL,&newtimer,nullptr);
}
int main(){
    signal(SIGALRM,f);
    set_time_interval(500);
    while(1){
        pause();
    }
}