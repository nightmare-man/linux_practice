/*
    bc程序接受输入，将其转后序表达式 传给dc又dc计算
    再传给bd，因此需要两个管道，实现两个方向传送
     父进程模拟bc 子进程打开dc
*/
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;
void dc_f(int in[2],int out[2]){
    //dup2这个调用，oldfd newfd ,将newfd也指向oldfd那个文件上
    //如果newfd原先指向的文件被打开了，那将被自动关闭
    dup2(in[0],0);
    dup2(out[1],1);
    close(in[1]);
    close(in[0]);
    close(out[0]);
    close(out[1]);
    int x=execlp("dc","dc",NULL);
    if(x==-1) throw runtime_error{" "};
    exit(1);
}
//bc为精简版，理论上需要接受用户输入的，但是我这儿写死了毕竟
//主要为了练习调用
void bc_f(int in[2],int out[2]){
    string s{"5 5 + p \n"};
    write(in[1],s.c_str(),s.size());
    char buf[10];
    read(out[0],buf,10);
    cout<<buf<<endl;
    exit(1);
}
int main(){
    int to_p[2]{};
    int from_p[2]{};
    pipe(to_p);
    pipe(from_p);
    int pid=fork();
    if(pid==0){
        //子进程
        dc_f(from_p,to_p);
    }else{
        //父进程
        bc_f(from_p,to_p);
    }
}