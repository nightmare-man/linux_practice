#include <iostream>
#include <fstream>
#include <iterator>
using namespace std;

constexpr int BUFFER_SIZE=1024;
int main(int ac,char* av[]){
    if(ac<3){
        cout<<"need 2 argument"<<endl;
        return 0;
    }
    //ifstream 和ofstream都带用户层缓冲
    ifstream from{av[1],ios::binary};
    ofstream to{av[2],ios::out|ios::trunc};
    if(!from) throw runtime_error("can't open file"+string{av[1]});
    if(!to) throw runtime_error("can't open/create file"+string{av[1]});
    char buf[BUFFER_SIZE]={};
   
    while(true){
         int cnt=from.readsome(buf,BUFFER_SIZE);
         to.write(buf,cnt);
         if(cnt<BUFFER_SIZE) break; 

    }
    return 0;
}