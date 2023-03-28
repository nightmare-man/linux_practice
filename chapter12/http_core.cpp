#include "http_core.h"
#include <iostream>
#include <string.h>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
using namespace std;
void forbidden(const char* msg);
void do_cat(const char* file);
void exec_cgi(const char* file);
bool end_in_cgi(const char* file);
void ls_dir(const char* dir);
void not_found(const char*msg);
HttpCore::HttpCore():file_info{nullptr}{
    char text[MAXLINELEN];
    cin.getline(text,MAXLINELEN);
    istringstream in{text};
    char split=0;
    in>>cmd_str>>split>>arg_str;
    file_info=new dirent;
    file_info->d_ino=0;
}
int HttpCore::set_file_info(){
    int ret=0;
    auto p=opendir(".");
    for(dirent* x;x=readdir(p);){
        if(strcmp(x->d_name,arg_str)==0){
            copy(x,x+1,file_info);
            return ret;
        }
    }
    return -1;
}
void HttpCore::Exec(){
    set_file_info();
    if(strcmp(cmd_str,"GET")!=0) forbidden("This cmd not implent now\r\n");
    else if(file_info->d_ino==0) not_found("This file not exist\r\t");
    else if(file_info->d_type==DT_DIR) ls_dir(arg_str);
    else if(end_in_cgi(arg_str)) exec_cgi(arg_str);
    else do_cat(arg_str);
}
void do_cat(const char* file){
    cout<<"HTTP/1.0 200 OK\r\n";
    cout<<"Content-type: text/plain\r\n";
    cout<<"\r\n";
    cout.flush();
    execl("/usr/bin/cat","/usr/bin/cat",file,nullptr);
}
void exec_cgi(const char* file){

}
bool end_in_cgi(const char* file){
    int len=strlen(file);
    if(len<4) return false;
    return strcmp(".cgi",file+len-4)==0;
}
void ls_dir(const char* dir){
    cout<<"HTTP/1.0 200 OK\r\n";
    cout<<"Content-type: text/plain\r\n";
    cout<<"\r\n";
    cout.flush();
    execl("/usr/bin/ls","/usr/bin/ls","-al",dir,nullptr);
}
void forbidden(const char* msg){
    cout<<"HTTP/1.0 501 Not Implemented\r\n";
    cout<<"Content-type: text/plain\r\n";
    cout<<"\r\n";
    cout<<msg;
    cout.flush();
}
void not_found(const char*msg){
    cout<<"HTTP/1.0 404 Not Foud\r\n";
    cout<<"Content-type: text/plain\r\n";
    cout<<"\r\n";
    cout<<msg;
    cout.flush();
}