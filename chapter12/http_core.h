
#pragma once
#include <dirent.h>
#define MAXLINELEN 128
class HttpCore{
    public:
    HttpCore();
    ~HttpCore(){delete file_info;};
    void Exec();
    private:
    int set_file_info();
    char cmd_str[10];
    char arg_str[MAXLINELEN];
    struct dirent* file_info;
};