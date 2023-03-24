#include <termios.h>
#include <unistd.h>
#include <iostream>
using namespace std;

int main(){
    struct termios info;
    //终端驱动程序控制四种操作，输入流的操作，输出流的操作，控制程序的操作，内部字符的操作
    //这里要查看终端输入是否开启回显，因此将STDIN_FILENO传入作为fd
    int rv=tcgetattr(STDIN_FILENO,&info);
    if(rv) {
        cout<<"无法获取终端属性";
        return -1;
    }
    int flag=info.c_lflag&ECHO;
    if(flag) cout<<"输入流开启了回显"<<endl;
    else cout<<"输入流未开启回显"<<endl;
    return 0;
}