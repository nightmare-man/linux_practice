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
void show_file_info(const char* fp){
    struct stat tmp;
    int ret=stat(fp,&tmp);
    if(ret) throw runtime_error("can't get info ");
    char* time=ctime(&tmp.st_mtim.tv_sec);\
    cout<<fp<<"\n";
    cout<<setw(8)<<"mode:\t"<<tmp.st_mode<<"\n";
    cout<<setw(8)<<"links:\t"<<tmp.st_nlink<<"\n";
    cout<<setw(8)<<"user:\t"<<tmp.st_uid<<"\n";
    cout<<setw(8)<<"size:\t"<<tmp.st_size<<"\n";
    cout<<setw(8)<<"group:\t"<<tmp.st_gid<<"\n";
    cout<<setw(8)<<"time:\t"<<time<<"\n";
    return;
}
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
        cout<<setw(8)<<setiosflags(ios::left)<<x->d_name<<endl;
    }
    return;
}
int main(int ac,char*av[]){
    if(ac<2) return 0;
    while(ac>1) show_file_info(av[--ac]);//do_ls(av[--ac]);
    return 0;
}