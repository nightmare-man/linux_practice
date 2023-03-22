#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <errno.h>
using namespace std;
int main(int ac,char*av[]){
    if(ac<2) return 0;
    DIR* s=opendir(av[1]);
    if(!s) {
        cout<<"can't open dir :"<<av[1]<<endl;
        return -1;
    }
    for(dirent* p=nullptr;p=readdir(s);){
        cout<<p->d_name<<endl;
    }
    return 0;
}