#pragma once
#include <curses.h>
#include <signal.h>
#include "set_ticker.h"
#include <iostream>
#define BLANK ' '
#define DFL_SYMBOL 'o'
#define TOP_ROW 5
#define BOT_ROW 20
#define LEFT_EDGE 10
#define RIGHT_EDGE 70
#define X_INIT 10
#define Y_INIT 10
#define TICKS_PER_SEC 50
#define X_TTM 8
#define Y_TTM 8
struct ppball{
    int y_pos;
    int x_pos;
    int y_ttm;//计时器复位初始值
    int x_ttm;
    int y_ttg;//计时器
    int x_ttg;
    int y_dir;
    int x_dir;
    char symbol;
};