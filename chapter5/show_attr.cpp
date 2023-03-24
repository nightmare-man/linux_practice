#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <string>
using namespace std;
class Terminal{
    public:
    struct flag_info{
        int value;
        string info;
    };
    Terminal::flag_info input_flags[10]={
        {IGNBRK,"IGnore break condition"},
        {BRKINT,"Signal interrupt on break"},
        {PARMRK,"Mark parity erros"},
        {INPCK,"Enable inout parity check"},
        {ISTRIP,"Strip character"},
        {INLCR,"Map NL to CR on input"},
        {IGNCR,"Ignore CR"},
        {ICRNL,"Map CR to NL on input"},
        {IXON,"Enable start/stop output control"},
        {IXOFF,"Enable start/stop input control"}
    } ;
    Terminal(int file_des):fd{file_des}{
        int rv=tcgetattr(file_des,&info);
        if(rv) throw runtime_error{"can't get attr"};
    }
    void show_baud(){
        int baud=0;
        if(fd==STDIN_FILENO){
            baud=cfgetispeed(&info);
        }else{
            baud=cfgetospeed(&info);
        }
        switch (baud)
        {
        
        case B2400:
            baud=2400;
            break;
        case B4800:
            baud=4800;
            break;
        case B9600:
            baud=9600;
            break;
        case B38400:
            baud=38400;
            break;
        default:
            break;
        }
        cout<<baud<<endl;
    }
    void show_some_flags(){
        int val=info.c_iflag;
        for(int i=0;input_flags[i].value;i++){
            cout<<input_flags[i].info<<" is ";
            if(input_flags[i].value&val){
                cout<<"on\n";
            }else{
                cout<<"off\n";
            }
        }
    }
    private:
    int fd;
    struct termios info;
};

int main(){
    Terminal t{STDIN_FILENO};
    t.show_baud();
    t.show_some_flags();
}