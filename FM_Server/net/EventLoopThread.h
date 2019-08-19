#ifndef EVENTLOOPTHREAD_H 
#define EVENTLOOPTHREAD_H 

#include <functional>
#include "../base/Thread.h"
#include "../base/Condition.h"
#include "../base/Mutex.h"
#include "EventLoop.h"

namespace FM_Server
{
namespace net
{
class EventLoopThread
{
    public:
        EventLoopThread();
        ~EventLoopThread();
        
        EventLoop* startLoop();
    private:
        void threadFun();

        Mutex mutex_;
        Thread thread_;
        Condition condition_;
        EventLoop* loop_;
};
}
}
#endif
