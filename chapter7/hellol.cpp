//该程序需要用-l ncurses命令来编译，链接ncurses库
#include  <iostream>
#include <curses.h>
#include <unistd.h>
using namespace std;
int main(){
    initscr();
    for(int i=0;i<LINES;i++){
        for(int j=0;j<COLS;j++){
            clear();
            move(i,j);
            addch('X');
            refresh();
        }
    }    
    getch();
    endwin();
}
