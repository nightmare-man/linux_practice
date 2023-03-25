//该程序需要用-l ncurses命令来编译，链接ncurses库
#include  <iostream>
#include <curses.h>
#include <unistd.h>
using namespace std;
int main(){
    initscr();
    for(int i=0;i<LINES;i++){
        move(i,i+1);
        if(i%2==1) standout();
        addstr("this");
        if(i%2==1) standend();
        sleep(1);
        refresh();
    }    
    getch();
    endwin();
}
