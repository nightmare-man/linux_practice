#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <iostream>
#include <errno.h>
#include <vector>
#include <algorithm>
#include <string.h>
#include <string>
#include <iomanip>
#include <time.h>
#include <pwd.h>
#include <grp.h>
using namespace std;
//stat结构体里的st_mode字段是一个16位数，用位来编码
//信息，因此要用掩码与运算获取对应位，定义在stat.h里

/// @brief 根据uid查找对应group并返回信息
/// @param uid 
string gid_to_name(int gid){
    group* p=getgrgid(gid);
    if(!p) throw runtime_error{"该uid对应的组无法找到"};
    return string{p->gr_name};
}

/// @brief 根据uid返回对应用户的信息
/// @param uid 
string uid_to_name(int uid){
    passwd* p=getpwuid(uid);
    if(!p) throw runtime_error{"没有该uid"};
    return string{p->pw_name};
}
/// @brief 根据给定的mode，用掩码来解读16个位，高4位是文件类型中间三位是suid sgid sticky，低9位分别是用户 组 其他人的读写执行权限
/// @param mode 位数据
/// @return 返回的权限字符串
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
/// @brief 根据输入的文件路径显示文件信息
/// @param fp 文件路径
void show_file_info(const char* fp){
    struct stat tmp;
    int ret=stat(fp,&tmp);
    if(ret) throw runtime_error("can't get info ");
    char* time=ctime(&tmp.st_mtim.tv_sec);\
    cout<<show_mode_info(tmp.st_mode);
    cout<<setw(5)<<tmp.st_nlink;
    cout<<setw(8)<<uid_to_name(tmp.st_uid);
    cout<<setw(8)<<gid_to_name(tmp.st_gid);
    cout<<setw(8)<<tmp.st_size;
    cout<<setw(8)<<time<<"\n";
    return;
}
/// @brief 根据文件夹路径显示名下的目录项
/// @param dir 文件夹路径
void do_ls(const char* dir){
    DIR* s=opendir(dir);
    if(!s) {
        cout<<"can't open dir :"<<dir<<endl;
        return;
    }
    vector<dirent*> v{};
    for(dirent* p=nullptr;p=readdir(s);){
        v.push_back(p);
    }
    sort(v.begin(),v.end(),[](dirent* a,dirent* b){
        return strcmp(a->d_name,b->d_name)<0;
    });
    for(auto x:v){
        cout<<setw(12)<<setiosflags(ios::left)<<x->d_name;
        show_file_info((string{dir}+string{x->d_name}).c_str() );
    }
    return;
}
/*
如何在什么头绪都没有的情况下找到该用什么系统调用？并编程？
比如要获取当前cpu占用情况，
两个方向 1 有关cpu的信息的调用 2有关进程信息调用
使用 man -k cpu | grep info
man -k process | grep info
来找到有关的系统调用（2是系统调用 3是库）
找到readproctab后再使用
man 3 readprctab来查看该调用的说明
*/
int main(int ac,char*av[]){
    if(ac<2) return 0;
    while(ac>1) do_ls(av[--ac]);
    return 0;
}