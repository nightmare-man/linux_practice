#include <pthread.h>
#include <signal.h>
#include <iostream>
#include <fstream>
using namespace std;

struct args{
    char* fname{nullptr};
    int number{-1};
};
struct args* finished=nullptr;
pthread_cond_t cond1;
pthread_cond_t cond2;
pthread_mutex_t mutex;
void* counter_word(void* arg){
    struct args* ag=reinterpret_cast<struct args*>(arg);
    ifstream in{ag->fname};
    if(!in) throw runtime_error{"can't open file"};
    int cnt=0;
    for(string s{};in>>s;){
        cnt++;
    }
    ag->number=cnt;
    pthread_mutex_lock(&mutex);
    finished=ag;
    pthread_cond_signal(&cond1);
    pthread_mutex_unlock(&mutex);
   
    return nullptr;
}


int main(int ac,char*av[]){
    if(ac!=3) return -1;
    struct args ag[2];
    ag[0].fname=av[1];
    ag[1].fname=av[2];
    pthread_mutex_init(&mutex,nullptr);
    pthread_cond_init(&cond1,nullptr);
    pthread_cond_init(&cond2,nullptr);
    pthread_t t1,t2;
    pthread_create(&t1,nullptr,counter_word,reinterpret_cast<void*>(ag));
    pthread_create(&t2,nullptr,counter_word,reinterpret_cast<void*>(ag+1));
    int cnt=0;
    while(cnt<2){
        pthread_mutex_lock(&mutex);
        while(finished==nullptr)
            pthread_cond_wait(&cond1,&mutex);
        cout<<"count "<<cnt<<":"<<finished->number<<endl;
        if(finished==ag) pthread_join(t1,nullptr);
        if(finished==ag+1) pthread_join(t2,nullptr);
        finished=nullptr;
        pthread_mutex_unlock(&mutex);
        cnt++;
    }
    cout<<"total wordls:"<<ag[1].number+ag[0].number<<endl;
}