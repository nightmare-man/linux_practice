#include <iostream>
#include <signal.h>
#include <unistd.h>
using namespace std;
void inthandler(int signal_no){
    cout<<"called with signal : "<<signal_no<<endl;
    sleep(10);
    cout<<"done handling signal : "<<signal_no<<endl;
}
int main(){
    struct sigaction newhandler;
    sigset_t blocked;//设置处理信号时阻塞哪些信号
    newhandler.sa_handler=inthandler;
    //不可重入，即当int信号处理时，该信号量处理被重置
    //因此第一次int信号被处理，第二次使用默认的处理即终止进程
    newhandler.sa_flags=SA_RESETHAND;
    sigemptyset(&blocked);
    sigaddset(&blocked,SIGQUIT);//int信号处理时阻塞SIGQUIT
    newhandler.sa_mask=blocked;
    sigaction(SIGINT,&newhandler,nullptr);
    while(1){
        
    }
    return 0;
}