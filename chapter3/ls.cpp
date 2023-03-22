#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <iostream>
#include <errno.h>
#include <vector>
#include <algorithm>
#include <string.h>
#include <iomanip>
#include <time.h>
using namespace std;
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
        struct stat tmp;
        string s1{dir};
        string s2{x->d_name};
        int ret=stat((s1+s2).c_str(),&tmp);
        if(ret) throw runtime_error("can't get info ");
        char* time=ctime(&tmp.st_mtim.tv_sec);
        cout<<setw(8)<<setiosflags(ios::left)<<x->d_name<<"\t"<<time<<endl;
    }
    return;
}
int main(int ac,char*av[]){
    if(ac<2) return 0;
    while(ac>1) do_ls(av[--ac]);
    return 0;
}