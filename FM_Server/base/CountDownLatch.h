#ifndef COUNTDOWNLATCH_H
#define COUNTDOWNLATCH_H

#include<unistd.h>
#include"Mutex.h"
#include"Condition.h"
namespace FM_Server
{
class CountDownLatch
{
    public:
    explicit CountDownLatch(int count):count_(count),mutex_(),cond_(mutex_){};
    ~CountDownLatch(){};
    void Wait();
    void CountDown();
    int GetCount();
    private:
    int count_;
    Mutex mutex_;
    Condition cond_;
};

}
#endif

