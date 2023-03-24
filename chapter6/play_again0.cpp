/*
    此版本实现了根据用户在终端输入
    的字符中读取YN返回0 1的功能，需要按下回车，有缓存
    
*/

#include <iostream>
#include <string>
#define QUESTION "Do u want another transaction"
using namespace std;
int get_response(const string& s){
    cout<<s<<"(y/n)?\n";
    while(1){
        switch (cin.get())
        {
        case 'Y':
        case 'y':
            return 0;
        case 'N':
        case 'n':
        case EOF:
            return 1;
        }
    }
}
int main(){
    int response=get_response(QUESTION);
    return response;
}