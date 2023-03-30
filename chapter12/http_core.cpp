#include "http_core.h"
#include <iostream>
#include <string.h>
#include <sstream>
#include <fstream>
#include <iterator>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <iomanip>
#include <sys/stat.h>
#include <time.h>
using namespace std;
void forbidden(char*,int,const char*);
void do_cat(char*,int,const char*);
void exec_cgi(char*,int,const char* );
bool end_in_cgi(const char*);
void ls_dir(char*,int,const char*);
void not_found(char*,int,const char*);
string show_mode_info(int mode);
HttpCore::HttpCore(const char* rq_str){
    istringstream in{rq_str};
    char split=0;
    in>>r.cmd>>split>>r.url;
    f_info.d_ino=0;
}
int HttpCore::set_file_info(){
    int ret=0;
    auto p=opendir(".");
    for(dirent* x;x=readdir(p);){
        if(strcmp(x->d_name,r.url.c_str())==0){
            copy(x,x+1,&f_info);
            closedir(p);
            return ret;
        }
    }
    closedir(p);
    return -1;
}
void HttpCore::Exec(char* buf,int len){
    set_file_info();
    if(strcmp(r.cmd.c_str(),"GET")!=0) forbidden(buf,len,"This cmd not implent now\r\n");
    else if(f_info.d_ino==0) not_found(buf,len,"This file not exist\r\t");
    else if(f_info.d_type==DT_DIR) ls_dir(buf,len,r.url.c_str());
    else if(end_in_cgi(r.url.c_str())) exec_cgi(buf,len,r.url.c_str());
    else do_cat(buf,len,r.url.c_str());
}
void do_cat(char* buf,int len,const char* file){
    ostringstream out{};
    out<<"HTTP/1.0 200 OK\r\n";
    out<<"Content-type: text/plain\r\n";
    out<<"\r\n";
    std::ifstream xin{file};
    if(!xin) throw runtime_error{"can't open file"};
    istream_iterator<char> in_begin{xin};//in_begin和传入流的seek有关
    istream_iterator<char> in_eof{};
    ostream_iterator<char> out_begin{out};
    copy(in_begin,in_eof,out_begin);
    strncpy(buf,out.rdbuf()->str().c_str(),len);
}
void exec_cgi(char*buf,int len,const char* file){

}
bool end_in_cgi(const char* file){
    int len=strlen(file);
    if(len<4) return false;
    return strcmp(".cgi",file+len-4)==0;
}
void ls_dir(char* buf,int len,const char* dir){
    ostringstream out{};
    out<<"HTTP/1.0 200 OK\r\n";
    out<<"Content-type: text/plain\r\n";
    out<<"\r\n";
    auto p=opendir( dir);
    for(dirent* x;x=readdir(p);){
        struct stat tmp;
        if(stat(x->d_name,&tmp)==-1) throw runtime_error{"stat error"};
        time_t tmp_t=time(nullptr);
        out<<setw(12)<<x->d_name<<show_mode_info(tmp.st_mode)<<"\t"<<ctime(&tmp_t)<<"\r\n";
    }
    strncpy(buf,out.rdbuf()->str().c_str(),len);
}
void forbidden(char*buf,int len,const char* msg){
    ostringstream out{};
    out<<"HTTP/1.0 501 Not Implemented\r\n";
    out<<"Content-type: text/plain\r\n";
    out<<"\r\n";
    out<<msg;
    strncpy(buf,out.rdbuf()->str().c_str(),len);
    
}
void not_found(char*buf,int len,const char*msg){
     ostringstream out{};
    cout<<"HTTP/1.0 404 Not Foud\r\n";
    cout<<"Content-type: text/plain\r\n";
    cout<<"\r\n";
    cout<<msg;
    strncpy(buf,out.rdbuf()->str().c_str(),len);
}
string show_mode_info(int mode){
    string priv_str{"---------"};
    int filetype=mode&S_IFMT;
    switch (filetype)
    {
    case S_IFDIR:
        priv_str[0]='d';
        break;
    case S_IFBLK:
        priv_str[0]='b';
        break;
    case S_IFREG:
        priv_str[0]='f';
    default:
        break;
    }
    if(mode&S_IRUSR) priv_str[1]='r';
    if(mode&S_IWUSR) priv_str[2]='w';
    if(mode&S_IXUSR) priv_str[3]='x';
    if(mode&S_IRGRP) priv_str[4]='r';
    if(mode&S_IWGRP) priv_str[5]='w';
    if(mode&S_IXGRP) priv_str[6]='x';
    if(mode&S_IROTH) priv_str[7]='r';
    if(mode&S_IWOTH) priv_str[8]='w';
    if(mode&S_IXOTH) priv_str[9]='x';
    return priv_str;
}