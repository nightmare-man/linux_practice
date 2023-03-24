#include <termios.h>
#include <unistd.h>
#include <iostream>
using namespace std;

int main(int ac,char*av[]){
    if(ac<2) return -1;
    bool flag=av[1][0]=='y';
    struct termios info;
    //终端驱动程序控制四种操作，输入流的操作，输出流的操作，控制程序的操作，内部字符的操作
    //这里要查看终端输入是否开启回显，因此将STDIN_FILENO传入作为fd
    //通过man -k terminal | grep attr可以找到这两个系统调用
    int rv=tcgetattr(STDIN_FILENO,&info);
    if(rv) {
        cout<<"无法获取终端属性";
        return -1;
    }
    if(flag)info.c_lflag|=ECHO;
    else info.c_lflag&=( ~ECHO );
    //写回属性
    tcsetattr(STDIN_FILENO,TCSANOW,&info);
    return 0;
}