#include "EventLoopThread.h"

FM_Server::net::EventLoopThread::EventLoopThread():
    mutex_(),
    thread_(std::bind(&EventLoopThread::threadFun,this)),
    condition_(mutex_),
    loop_(NULL)
{}

FM_Server::net::EventLoopThread::~EventLoopThread()
{}

void FM_Server::net::EventLoopThread::threadFun()
{
    EventLoop eventLoop;
    {
        MutexGuard mutexGuard(mutex_);
        loop_=&eventLoop;
        condition_.notifyall();
    }
    loop_->loop();
    loop_=NULL;
}

FM_Server::net::EventLoop* FM_Server::net::EventLoopThread::startLoop()
{
    thread_.start();
    {
        MutexGuard mutexGuard(mutex_);
        while(loop_==NULL)
        {
            condition_.wait();
        }
    }
    return loop_;

}
