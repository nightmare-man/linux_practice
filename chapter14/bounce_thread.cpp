#include <iostream>
#include <pthread.h>
#include <curses.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
using namespace std;
#define SYMBOL "hello world"
#define BLANK  "           "
#define SPEED 20//1秒3格
#define FPS 60
#define HIGH 10
#define WIDTH 20
struct position{
    int x{0};
    int y{0};
    int x_dir{1};
    int y_dir{1};
};
struct position state;
pthread_mutex_t mutex;

void* work_handler(void* arg){
    int interval_ms=1000/FPS;
    int move_frame= 1000/(SPEED*interval_ms);
    int cnt=0;
    int str_len=strlen(SYMBOL);
    while(true){
        if(cnt>=move_frame){
            mvaddstr(state.y,state.x,BLANK);
            pthread_mutex_lock(&mutex);
            state.x+=state.x_dir;
            state.y+=state.y_dir;
            if(state.x<=0){
                state.x_dir=1;
            }else if(state.x>=COLS-str_len){
                state.x_dir=-1;
            }
            if(state.y<=0){
                state.y_dir=1;
            }else if(state.y>=LINES-1){
                state.y_dir=-1;
            }
            pthread_mutex_unlock(&mutex);
            mvaddstr(state.y,state.x,SYMBOL);
            cnt=0;
        }
        cnt++;
        move(LINES-1,COLS-4);
        refresh();
        usleep(interval_ms*1000);
    }
}
int main(){
    initscr();
    crmode();//不需要flush即可写入
    noecho();//不回显
    pthread_t work;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    pthread_create(&work,&attr,work_handler,nullptr);
    pthread_detach(work);
    while(true){
        char c=0;
        c=cin.get();
        if(c=='q') break;
        else if(c=='w'){
            pthread_mutex_lock(&mutex);
            state.y_dir=-1;
            pthread_mutex_unlock(&mutex);
        }
        else if(c=='s'){
            pthread_mutex_lock(&mutex);
            state.y_dir=1;
            pthread_mutex_unlock(&mutex);
        }
        else if(c=='a'){
            pthread_mutex_lock(&mutex);
            state.x_dir=-1;
            pthread_mutex_unlock(&mutex);
        }
        else if(c=='d'){
            pthread_mutex_lock(&mutex);
            state.x_dir=1;
            pthread_mutex_unlock(&mutex);
        }
    }
    endwin();
    return 0;
}