#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <errno.h>
#include <vector>
#include <algorithm>
#include <string.h>
using namespace std;
void do_ls(const char* name){
    DIR* s=opendir(name);
    if(!s) {
        cout<<"can't open dir :"<<name<<endl;
        return;
    }
    vector<dirent*> v{};
    for(dirent* p=nullptr;p=readdir(s);){
        v.push_back(p);
    }
    sort(v.begin(),v.end(),[](dirent* a,dirent* b){
        return strcmp(a->d_name,b->d_name)<0;
    });
    for(auto x:v) cout<<x->d_name<<endl;
    return;
}
int main(int ac,char*av[]){
    if(ac<2) return 0;
    while(ac>1) do_ls(av[--ac]);
    return 0;
}