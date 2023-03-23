#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <sstream>
#include <errno.h>
#include <algorithm>
using namespace std;
int main(int ac,char* av[]){
    if(ac<2) return -1;
    cin>>noskipws;
    istream_iterator<char> in{cin};
    istream_iterator<char> in_eof{};
    ofstream to{string{"/dev/pts/"}+string{av[1]}};
    if(!to){
        cout<<"open file error :"<<errno<<endl;
        return -1;
    }
    ostream_iterator<char> out{to};
    copy(in,in_eof,out);
    cin>>skipws;
    return 0;
}