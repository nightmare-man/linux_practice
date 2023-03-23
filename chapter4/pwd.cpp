#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
using namespace std;

string get_complete_path(){
   string ret{};
   vector<string> path{};
   ino_t curr=0,parent=0;
   auto p=opendir(".");
   if(!p) return ret;
   for(dirent* x=nullptr;x=readdir(p);){
        //字符串不能直接比较
        if(strcmp(x->d_name,".")==0) curr=x->d_ino;
        if(strcmp(x->d_name,"..")==0) parent=x->d_ino;
   }
   while(true){
        p=opendir("..");
        if(!p) break;
        ino_t tmp=curr;
        for(dirent* x=nullptr;x=readdir(p);){
            if(x->d_ino==tmp) {
                path.push_back(string{x->d_name});
                path.push_back("/");
            }
            if(strcmp(x->d_name,"..")==0) {
                curr=parent;
                parent=x->d_ino;
            }    
        }
        closedir(p);
        if(curr==parent) break;
        chdir("..");
   }
   for(int i=path.size()-1;i>=0;i--){
    ret+=path[i];
   }
   return ret;
}
int main(){
    cout<<get_complete_path()<<endl;
}