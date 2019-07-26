#include"Mutex.h"
#include"Condition.h"
#include"CountDownLatch.h"
using namespace FM_Server;

void CountDownLatch::Wait() 
{
    MutexGuard lock(mutex_);
    while(count_>0)
        cond_.wait();
}

void CountDownLatch::CountDown()
{
    MutexGuard lock(mutex_);
    --count_;
    if(count_==0)
        cond_.notifyall();
}

int CountDownLatch::GetCount()
{
    MutexGuard lock(mutex_);
    return count_;
}
