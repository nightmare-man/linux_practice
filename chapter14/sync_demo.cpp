#include <pthread.h>
#include <signal.h>
#include <iostream>
#include <fstream>
using namespace std;
//实现目标，两个线程counter_word，不论谁完成立马通知主线程输出结果，最后求和

//思路零，直接用两个join,但是join有先后之分，必须等到前一个join结束
//才能执行后一个join,因此无法实现先完成的主线程先处理。

//思路一：通过两个局部变量，分别表示完成状态，主线程轮询

//思路二：不使用无脑轮询，而是用条件变量，这种情况不可能用pthread_mutex
//实现，原因是这种锁的lock和unlock必须成对出现，不允许没拿到锁而直接释放锁
//因此无法实现单方面的阻塞和唤醒其他线程。
//条件变量的使用， 分析关系，①主线程等任意一个线程计数完毕
//②技术线程通知主线程时，主线程必须在等待状态，不然“唤醒丢失”
//关系一用一个条件变量即可实现，因为如果是两个条件变量，那先等谁都不行，不能
//实现等待任意，应该用一个条件变量等两次
//关系二 要等主线程到等待状态又得使用条件变量。
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
    int finish[2]{0,0};
    while(cnt<2){
        if(ag[0].number!=-1&&finish[0]==0){
            cout<<"file1 words: "<<ag[0].number<<endl;
            cnt++;
            finish[0]=1;
            //join没啥用 只为了拿到返回值，但这里不需要用到返回值
            //用的是参数里的number
            pthread_join(&t1,nullptr);
        }
        if(ag[1].number!=-1&&finish[1]==0){
            cout<<"file2 words: "<<ag[1].number<<endl;
            cnt++;
            finish[1]=1;
            pthread_join(&t2,nullptr);
        }
    }
    cout<<"total wordls:"<<ag[1].number+ag[0].number<<endl;
}