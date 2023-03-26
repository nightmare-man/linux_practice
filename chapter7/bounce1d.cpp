#include <curses.h>
#include <iostream>
#include <signal.h>
#include <sys/time.h>
#include "set_ticker.h"
#define MESSAGE "HELLO"
#define MESSAGE_LEN 5
#define BLANK   "     "//将原来的单词擦掉
int row=0;
int col=0;
int dir=0;//方向
using namespace std;


//显然这个处理函数是不可重入的，因此无法递归，处理只能阻塞同种信号
//row col是全局变量 addstr也是全局生效的
//举个例子当速度快到一定程度，出现递归调用的时候会refresh执行了
//但是后面的判断反转来不及执行，因此col的位置会越界（一直朝一个方向）
void move_msg(int sig_no){
    signal(SIGALRM,move_msg);
    move(row,col);
    addstr(BLANK);//覆盖原来的
    col+=dir;
    move(row,col);
    addstr(MESSAGE);
    refresh();
    if(dir==-1&&col<=0) dir=1;//反转
    else if(dir==1&&(col+ MESSAGE_LEN)>=COLS-1) dir=-1;
}
int main(){
    int delay=0;
    int ndelay=0;
    char c=0;
    initscr();
    crmode();//crmode即非常规模式，不需等待刷新
    noecho();
    clear();
    row=10;
    col=0;
    dir=1;
    delay=200;
    move(row,col);
    addstr(MESSAGE);
    signal(SIGALRM,move_msg);
    set_ticker(delay);
    while(1){
        ndelay=0;
        c=cin.get();
        bool flag=true;
        switch (c)
        {
        case 'Q':
            flag=false;
            break;
        case ' ':
            dir=-dir;
            break;
        case 'f':
            if(delay>2) ndelay=delay/2;
            break;
        case 's':
            ndelay=2*delay;
            break;
        
        default:
            break;
        }
        delay=ndelay;
        if(ndelay>0) set_ticker(ndelay);
        if(!flag) break;
    }
    endwin();
    return 0;
}