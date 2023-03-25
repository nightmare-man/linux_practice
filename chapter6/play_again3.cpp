/*
    此版本实现了根据用户在终端输入
    的字符中读取YN返回0 1的功能，通过设置终端的驱动程序，无缓冲
    不需要回车
    关闭回显
    非阻塞io
*/
#include <iostream>
#include <string>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#define QUESTION "Do u want another transaction"

#define SLEEP_TIME 2
using namespace std;

void set_nodelay(){
    int flag=fcntl(STDIN_FILENO,F_GETFL);
    //非阻塞模式，没读到也直接返回
    //O_NDELAY 已经废弃
    flag|=O_NONBLOCK;
    fcntl(0,F_SETFL,flag);
}

/// @brief 保存恢复tty设置
/// @param how 
void tty_mode(int how){
    //使用静态局部变量，第一次时初始化，全局唯一，作用范围局部
    static termios origin_mode;
    if(how==0) tcgetattr(0,&origin_mode);
    else if(how==1) tcsetattr(0,TCSANOW,&origin_mode);
}
void set_ncr_necho_mode(){
    termios mode;
    tcgetattr(0,&mode);
    //设置为非规范模式
    mode.c_lflag&= (~ICANON);
    mode.c_lflag&=(~ECHO);
    //终端一次读入1个字符
    mode.c_cc[VMIN]=1;
    //写回
    tcsetattr(0,TCSANOW,&mode);
}
char get_ok_char(){
    int c;
    while(true){
        c=getchar();
        if(c==0||c==-1) break;
        if(c=='y'||c=='Y'||c=='n'||c=='N') break;
    }
    return c;
}
int get_response(const string& s){
    char input=0;
    cout<<s<<"(y/n)?\n";
    cout.flush();
    int max_tries=3;
    while(1){
        sleep(SLEEP_TIME);
        input=get_ok_char();
        switch (input)
        {
        case 'Y':
        case 'y':
            return 0;
        case 'N':
        case 'n':
            return 1;
        default:
            {
                cout<<max_tries<<endl;
                max_tries--;
                if(max_tries==0) return 2;
                break;
            }
        }
        cout<<"\a";
    }
}

int main(){
    tty_mode(0);
    set_ncr_necho_mode();
    set_nodelay();
    int response=get_response(QUESTION);
    tty_mode(1);
    //如果正常退出时，终端会自动恢复
    //成阻塞模式，但是如果用ctrl+c
    //会杀死进程，导致终端没有退出非阻塞模式，因此shell会
    //卡住 待改进
    return response;
}