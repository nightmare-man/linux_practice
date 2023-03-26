#include <unistd.h>
#include <iostream>
using namespace std;
int main(){
    int len=0;
    int i=0;
    int apipe[2];//用于储存返回的描述符 0是输出 1是输入
    int ret=pipe(apipe);
    if(ret==-1) return -1;
    char buf=0;
    while(true){
        buf=cin.get();
        write(apipe[1],&buf,1);
        int ret=read(apipe[0],&buf,1);
        if(ret>0&&buf!=EOF)cout<<buf<<endl;
        else break;
    }
    return 0;
}