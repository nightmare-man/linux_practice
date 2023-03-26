#include "bounce2d.h"
struct ppball the_ball;
int bounce_or_lose(struct ppball* bp){
    int ret=0;
    if(bp->y_pos==TOP_ROW){
        bp->y_dir=1;
        ret=1;
    }else if(bp->y_pos==BOT_ROW){
        bp->y_dir=-1;
        ret=1;
    }
    if(bp->x_pos==LEFT_EDGE){
        bp->x_dir=1;
        ret=1;
    }else if(bp->x_pos==RIGHT_EDGE){
        bp->x_dir=-1;
        ret=1;
    }
    return ret;

}
void ball_move(int sig){
    int y_cur,x_cur,moved;
    signal(SIGALRM,SIG_IGN);
    y_cur=the_ball.y_pos;
    x_cur=the_ball.x_pos;
    moved=0;
    if(the_ball.y_ttm>0&&the_ball.y_ttg--==1){
        the_ball.y_pos+=the_ball.y_dir;
        the_ball.y_ttg=the_ball.y_ttm;
        moved=1;
    }
    if(the_ball.x_ttm>0&&the_ball.x_ttg--==1){
        the_ball.x_pos+=the_ball.x_dir;
        the_ball.x_ttg=the_ball.x_ttm;
        moved=1;
    }
    if(moved){
        //光标移动到指定位置输出字符串
        mvaddch(y_cur,x_cur,BLANK);
        mvaddch(the_ball.y_pos,the_ball.x_pos,the_ball.symbol);
        bounce_or_lose(&the_ball);
        //光标回右下角
        move(LINES-1,COLS-4);

       addstr(std::to_string(the_ball.x_ttm).c_str());
        refresh();
    }
    signal(SIGALRM,ball_move);
}
void set_up(){
     the_ball.y_pos=Y_INIT;
     the_ball.x_pos=X_INIT;
     the_ball.y_ttg=the_ball.y_ttm=Y_TTM;
     the_ball.x_ttg=the_ball.x_ttm=X_TTM;
     the_ball.y_dir=1;
     the_ball.x_dir=1;
     the_ball.symbol=DFL_SYMBOL;
     initscr();
     noecho();
     crmode();
     signal(SIGINT,SIG_IGN);
     mvaddch(the_ball.y_pos,the_ball.y_pos,the_ball.symbol);
     refresh();
     signal(SIGALRM,ball_move);
     set_ticker(1000/TICKS_PER_SEC);
}

void wrap_up(){
    set_ticker(0);
    endwin();
}

int main(){
    int c;
    set_up();
    while(true ){
        c=getch();
        if(c=='f'){
            the_ball.x_ttm--;
            the_ball.y_ttm--;
        }
        else if(c=='s'){
            the_ball.x_ttm++;
            the_ball.y_ttm++;
        }
        else if(c=='j') the_ball.x_dir=-1;
        else if(c=='l') the_ball.x_dir=1;
        else if(c=='i') the_ball.y_dir=-1;
        else if(c=='k') the_ball.y_dir=1;
        else if(c=='q') break;
    }
    wrap_up();
}