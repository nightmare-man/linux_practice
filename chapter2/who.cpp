#include <iostream>
#include <fstream>
#include <string.h>s
#include <utmp.h>
#include <iomanip>
#include <time.h>
#include <string>
using namespace std;
template<typename T>
const char* as_bytes(const T& t){
    return &(reinterpret_cast<const char&>(t));
}
template<typename T>
char* get_addr(T& t){
    return &(reinterpret_cast<char&>(t));
}
void show_time(time_t t){
    //根据man 3 ctime的结果 ctime返回的指针指向静态内存区 static的
    //不能free 不是线程安全
    char* p=ctime(&t);
    string s{p};
    cout<<s;
    return;
}
void show_info(const utmp& x){
    if(x.ut_type!=USER_PROCESS) return;
    cout<<setw(8)<<x.ut_user<<" ";
    show_time(time_t{x.ut_tv.tv_sec});
    cout<<setw(8)<<x.ut_line<<" "<<x.ut_host<<endl;
}
int main(){
    utmp current_record;  
    ifstream in{_PATH_WTMP,ios::binary};
    if(!in) throw runtime_error("can't open file\n");  
    while(in.read(get_addr<utmp>(current_record),sizeof(utmp))){
        show_info(current_record);
    }
    return 0;
}
