#include <iostream>
#include <fstream>
using namespace std;
constexpr int PAGELEN {24};
int see_more(){
    char c=0;
    cout<<"\033[7m more? \033[m]";
    //cin.get能接收换行符
    while(cin.get(c)){
        if(c=='q') return 0;
        if(c==' ') return PAGELEN;
        if(c=='\n') return 1;
    }
}
void do_more(istream& in){
    string line{""};
    int reply=0;
    int num_of_lines=0;
    while(getline(in,line)){
        if(num_of_lines==PAGELEN){
            reply=see_more();
            if(reply==0) break;
            num_of_lines-=reply;
        }
        cout<<line<<endl;
        num_of_lines++;
    }
}
int main(int ac,char* av[]){
    if(ac==1) do_more(cin);
    else{
        ifstream from{av[1]};
        if(!from) throw runtime_error{"can't open file"};
        do_more(from);
    }
    return 0;

}