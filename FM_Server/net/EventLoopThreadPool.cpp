#include "EventLoopThreadPool.h"

FM_Server::net::EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop,int threadNum):
    baseLoop_(baseLoop),
    threadNum_(threadNum),
    next_(0),
    isStart_(false)
{};

FM_Server::net::EventLoopThreadPool::~EventLoopThreadPool(){}

void FM_Server::net::EventLoopThreadPool::start()
{
    assert(baseLoop_->isInLoopThread());
    isStart_=true;
    for(int i=0;i<<threadNum_;i++)
    {
        EventLoopThreadPtr eventLoopThreadPtr;
        eventLoopThreadPtr.reset(new EventLoopThread());
        loops_.push_back(eventLoopThreadPtr->startLoop());
        eventLoopThreads_.push_back(std::move(eventLoopThreadPtr));
    }
}

FM_Server::net::EventLoop* FM_Server::net::EventLoopThreadPool::getNextLoop()
{
    assert(baseLoop_->isInLoopThread());
    EventLoop* loop=baseLoop_;
    if(!loops_.empty())
    {
        loop=loops_[next_];
        next_++;
        if(next_==static_cast<int>(loops_.size()))
        {
            next_=0;
        }
    }
    return loop;
}
