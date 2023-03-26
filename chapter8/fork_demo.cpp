#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>

using namespace std;

void int_handler(int sig){
    cout<<"int handler :"<<getpid()<<endl;
}
int main(){
    cout<<"parent pid :"<<getpid()<<endl;
    signal(SIGINT,int_handler);
    int ret=fork();
    while(1){

    }
    return 0;
}