/*
    此版本实现了根据用户在终端输入
    的字符中读取YN返回0 1的功能，通过设置终端的驱动程序，无缓冲
    不需要回车
    
*/

#include <iostream>
#include <string>
#include <termios.h>
#define QUESTION "Do u want another transaction"
using namespace std;

/// @brief 保存恢复tty设置
/// @param how 
void tty_mode(int how){
    //使用静态局部变量，第一次时初始化，全局唯一，作用范围局部
    static termios origin_mode;
    if(how==0) tcgetattr(0,&origin_mode);
    else if(how==1) tcsetattr(0,TCSANOW,&origin_mode);
}
void set_crmode(){
    termios mode;
    tcgetattr(0,&mode);
    //设置为非规范模式
    mode.c_lflag&= (~ICANON);
    //终端一次读入1个字符
    mode.c_cc[VMIN]=1;
    //写回
    tcsetattr(0,TCSANOW,&mode);
}
int get_response(const string& s){
    cout<<s<<"(y/n)?\n";
    while(1){
        switch (cin.get())
        {
        case 'Y':
        case 'y':
            return 0;
        case 'N':
        case 'n':
        case EOF:
            return 1;
        default:
        cout<<"\ncan not understand,type y or n\n";
        }
    }
}

int main(){
    tty_mode(0);
    set_crmode();
    int response=get_response(QUESTION);
    tty_mode(1);
    return response;
}