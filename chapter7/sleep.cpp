#include <iostream>
#include <signal.h>
using namespace std;
void wakeup(int p){
    cout<<"Alarm received\n";
}
int main(){
    signal(SIGALRM,wakeup);
    alarm(4);
    //pause 暂停进程，知道处理信号
    pause();
    cout<<"wake up\n";
}