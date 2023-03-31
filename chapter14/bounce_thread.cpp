/*
    根据不断实验，发现创建出来的线程的pause无法被信号唤醒
    不管信号是谁发出来的
    


    使用单独线程读取状态并更新ui，设置为60帧
    主线程初始化ui ，loop结束输入 修改状态
    因此状态是临界资源，这里仅设置一个状态那就是球的位置和运动方向;
    ui线程出了读状态外，这里偷懒也用这个线程改状态了，改状态加锁
    主线程改状态也加锁， 读就不加了，读理论上也要加的，这样才保证
    数据的一致性和读的正确性，现在不加，导致读出来的数据可能落后了
    落后了就落后了把，就落后一帧
*/
#include <iostream>
#include <curses.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include "../chapter7/set_ticker.h"
#define SYMBOL 'o'
#define BLANK  ' '
#define FPS 60 
#define XSPEED 4  //x轴每秒移动4格子 
#define YSPEED 6   
struct position{
    int x{0};
    int y{0};
    int x_dir{1};
    int y_dir{1};
};
struct position state;
pthread_mutex_t mutex;
void alarm_handle(int sig){
std::cout<<"handler"<<std::endl;
}
void* ui_update(void* arg){
    std::cout<<"begin"<<std::endl;
    signal(SIGALRM,alarm_handle);
    //alarm(1);
    while(true){
        //先把老状态清除
        state.x++;
        mvaddch(state.y,state.x,SYMBOL);
        
        //根据man的描述 pause会被唤醒直到进程收到信号将被终止，或者信号处理
        //函数被调用，与哪个线程调用信号处理函数无关，都能被唤醒
        pause();
        std::cout<<"alarmed"<<std::endl;
    }
}
int main(){
    //初始化ui
   
    
    pthread_t work;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    pthread_create(&work,&attr,ui_update,nullptr);
   // pthread_detach(work);
   alarm(4);
    while(true){
        
        /*
        char c=0;
        c=std::cin.get();
        if(c=='q'){
            break;
        }else if(c=='w'){
            pthread_mutex_lock(&mutex);
            state.y_dir=-1;
            pthread_mutex_unlock(&mutex);
        }else if(c=='s'){
            pthread_mutex_lock(&mutex);
            state.y_dir=1;
            pthread_mutex_unlock(&mutex);
        }else if(c=='a'){
            pthread_mutex_lock(&mutex);
            state.x_dir=-1;
            pthread_mutex_unlock(&mutex);
        }else if(c=='d'){
            pthread_mutex_lock(&mutex);
            state.x_dir=1;
            pthread_mutex_unlock(&mutex);
        }*/
    }
  
    return 0;
}