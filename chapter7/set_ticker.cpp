#include "set_ticker.h"
void set_ticker(int ms){
    int sec=ms/1000;
    int us=(ms%1000)*1000;
    struct itimerval newtime;
    newtime.it_interval.tv_sec=sec;
    newtime.it_interval.tv_usec=us;
    newtime.it_value.tv_sec=sec;
    newtime.it_value.tv_usec=us;
    setitimer(ITIMER_REAL,&newtime,nullptr);
}