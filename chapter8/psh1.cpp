#include <unistd.h>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>
#include <errno.h>
#define MAX_LINE 512
using namespace std;

int execute(const vector<string>& list ){
    //这里之前用数组出错了，猜测的原因时局部内置类型的变量没有初始化为0
    //导致execvp访问越界
    char** arg=new char* [list.size()];
    for(int i=0;i<list.size();i++){
        char* tmp=new char[list[i].length()+1];
        strcpy(tmp,list[i].c_str());
        arg[i]=tmp;
    }
    //根据man execve知，除了少数几种资源，所有进程内存空间均被替换
    //栈 堆 datasection codesection
    execvp(arg[0],arg);
    for(int i=0;i<list.size();i++){
        cout<<arg[i]<<endl;
    }
    cout<<"failed execvp :"<<errno<<endl;

    return 0;
}
int main(){
    vector<string> v{};
    cout<<"[psh]>";
    char buf[MAX_LINE];
    cin.getline(buf,MAX_LINE);
    istringstream is{buf};
    for(string p{};is>>p;){
        v.push_back(p);
    }
    execute(v);
    return 0;
}
