
#include "../chapter12/http_core.h"
#include <iostream>
using namespace std;
int main(int ac,char*av[]){
    if(ac<2) return -1;
    HttpCore core{"GET /test 200/OK\n"};
    char buf[4096];
    core.Exec(buf,4096);
    cout<<buf<<endl;
}