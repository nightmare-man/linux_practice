#include <pthread.h>
#include <signal.h>
#include <iostream>
#include <fstream>
using namespace std;
//实现目标，两个线程counter_word，不论谁完成立马输出结果，最后求和
//思路一：通过两个局部变量，分别表示完成状态，主线程轮询
struct args{
    char* fname{nullptr};
    int number{-1};
};
void* counter_word(void* arg){
    struct args* ag=reinterpret_cast<struct args*>(arg);
    ifstream in{ag->fname};
    if(!in) throw runtime_error{"can't open file"};
    int cnt=0;
    for(string s{};in>>s;){
        cnt++;
    }
    ag->number=cnt;
    return nullptr;
}


int main(int ac,char*av[]){
    if(ac!=3) return -1;
    struct args ag[2];
    ag[0].fname=av[1];
    ag[1].fname=av[2];
    pthread_t t1,t2;
    
    pthread_create(&t1,nullptr,counter_word,reinterpret_cast<void*>(ag));
    pthread_create(&t2,nullptr,counter_word,reinterpret_cast<void*>(ag+1));
    int cnt=0;
    //错误示范一：这里用轮询不断查两个当中是否完成，
    //并记录完成数，但实际上第二次查询时仍将之前完成的计入，导致直接出循环
    //主线程结束，导致另一个没完成的也结束了。完全不符合等到两个都完成并
    //立刻显示的目的。因此一旦发现完成，立刻显示并标记已被检查过下次不再检查
    while(cnt<2){
        if(ag[0].number!=-1){
            cout<<"file1 words: "<<ag[0].number<<endl;
            cnt++;
        }
        if(ag[1].number!=-1){
            cout<<"file2 words: "<<ag[1].number<<endl;
            cnt++;
        }
    }
    cout<<"total wordls:"<<ag[1].number+ag[0].number<<endl;
}