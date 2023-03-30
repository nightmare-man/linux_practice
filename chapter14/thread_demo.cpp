#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
/* 根据man 7 pthreads 编译时需要手动连接 phtread 所以要用-l pthread
    根据man 7 signal 有些信号是针对进程的 有些是针对特定信号的
    针对进程时，会随机选一个能处理未被阻塞的线程来处理

    多线程编码的两个关键问题， 1同步 2互斥
    互斥指有些资源不能同时访问，否则会出错
    同步指需要按照指定的顺序执行代码

    一  实现互斥 锁，锁有两种，1是睡眠的互斥锁，拿不到就阻塞，锁释放时随机唤醒一个阻塞的
    2是自旋锁，拿不到一直试图拿。两者各有优劣，前者需要系统调用阻塞就绪 后者loop 
    
    二 实现同步 条件变量和信号量解决， 
    
*/

using namespace std;
long counter=12;
pthread_mutex_t lock;
void* routine(void * p){
    for(long i=0;i<1000000;i++){
        pthread_mutex_lock(&lock);
        counter*=7;
        sleep(1);
        pthread_mutex_unlock(&lock);
    
    }
    return nullptr;
}
void* routine1(void* p){
    for(long i=0;i<1000000;i++){
        pthread_mutex_lock(&lock);
        counter*=7;
        pthread_mutex_unlock(&lock);
    }
    return nullptr;
}
int main(){
    pthread_t x1;//thread_id
    pthread_t x2;
    //第二个参数设置县城属性，为null即默认
    pthread_create(&x1,nullptr,routine,nullptr);
    pthread_create(&x2,nullptr,routine1,nullptr);

    //等待指定的线程技术，如果x2先结束，也得等x1结束才能继续往下
    //因为x1的等待在前面
    pthread_join(x1,nullptr);
    pthread_join(x2,nullptr);
    std::cout<<counter<<std::endl;
    return 0;
}