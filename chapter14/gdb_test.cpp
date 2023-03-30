#include <iostream>
#include <signal.h>
#include <unistd.h>
using namespace std;
void handler(int sig){
    cout<<"sig "<<sig<<endl;
}
int main(int ac,char*av[]){
    if(ac<2) return -1;
    signal(SIGINT,handler);
    while(1);
}