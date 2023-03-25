#include <signal.h>
#include <iostream>
using namespace std;
int flag=0;
void f(int p){
    cout<<"int occured"<<endl;
    flag=1;
    return ;
}
int main(){
    auto ret=signal(SIGINT,SIG_IGN);
    if( long(ret)==-1) cout<<"set error"<<endl;
    while(!flag){

    }
    return 0;
}