#pragma pack(1)
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <utmp.h>
#include <time.h>
#include <errno.h>
using namespace std;
constexpr int STRUCT_SIZE=sizeof(utmp);
template<typename T>
const char* as_bytes(const T& t){
    return &(reinterpret_cast<const char&>(t));
}
template<typename T>
char* get_addr(T& t){
    return &(reinterpret_cast<char&>(t));
}
int main(int ac,char*av[]){
    string s{"pts/12"};
    ifstream from{_PATH_WTMP,from.binary};
    if(!from.is_open()) throw runtime_error("can't open");
    utmp tmp;
    while( from.read(get_addr<utmp>(tmp),STRUCT_SIZE).good()&&\
        0!=strncmp( s.c_str(),tmp.ut_line,s.length())
    );
    int seek=from.tellg();
    from.close();
    tmp.ut_tv.tv_sec=time(nullptr);
    ofstream to{_PATH_WTMP,to.binary};
    //没权限往 目标文件写
    //使用errno来获取系统调用错误原因
    if(!to.is_open()){
        switch (errno)
        {
        case ENOENT:
            cout<<"no such file\n";
            break;
        case EINTR :
            cout<<"interrupted while opening file\n";
            break;
        case EACCES:
            cout<<"don't have permission\n";
            break;
        default:
            cout<<"unknow error while open file\n";
            break;
        }
    }
    to.seekp(seek-STRUCT_SIZE,to.beg);
    to.write(as_bytes<utmp>(tmp),STRUCT_SIZE);
    return 0;
}