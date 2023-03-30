
#pragma once
#include <dirent.h>
#include <string>
#define MAXLINELEN 128
class HttpCore{
    public:
    struct Request{
        std::string cmd;
        std::string url;
    };
    HttpCore(const char* request_str);
    void Exec(char*buf,int len);
    private:
    int set_file_info();
    struct Request r;
    struct dirent f_info;
};