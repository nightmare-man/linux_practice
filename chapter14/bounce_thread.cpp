#include <pthread.h>
#include <iostream>
#include <signal.h>
using namespace std;
//多线程中signal不能用，需要用sigwait 进行同步处理
void sig_handler(int sig){
}
void* work_handler(void* arg){
    signal(SIGALRM,sig_handler);
    while(true){
        alarm(4);
        pause();
        cout<<"work"<<endl;
    }
    return nullptr;
}
int main(){
    
    pthread_t work;
    pthread_create(&work,nullptr,work_handler,nullptr);
    while(true){
        
        pause();
        cout<<"main"<<endl;
    }
    return 0;
}