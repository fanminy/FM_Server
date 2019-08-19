#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "Channel.h"
#include "../base/CurrentThread.h"
#include "../log/Logger.h"
#include "../base/Mutex.h"
#include "Epoll.h"
#include <vector>
#include <functional>
#include <sys/eventfd.h>
#include <assert.h>
#include <algorithm>

namespace FM_Server
{
namespace net
{
class Channel;
class Epoll;
class EventLoop
{
    typedef std::function<void()> CallBackFun;
    public:
        EventLoop();
        ~EventLoop();
        void updateChannel(Channel* channel);
        void removeChannel(Channel* channel);
        void loop();

        void runInLoop(CallBackFun cb);
        void queueInLoop(CallBackFun cb);
        void wakeup();

        void quit();
        bool isInLoopThread() const {return currentThreadID_==CurrentThread::gettid();}
    private:
    typedef std::vector<Channel*> ChannelList;
    typedef std::function<void()> PendingFunc;

    ChannelList activeChannel_;
    Channel* currentActiveChannel_;
    bool runningTag_;
    bool quitTag_;
    bool isEventHanding_;

    //process pending function
    void doPendingFunctor();
    std::vector<PendingFunc> pendingFuncList_;
    bool callPendingFuncTag_;

    //use to wake up thread while thread is blocking is epoll_wait
    int wakeupfd_;
    void handleRead();
    std::unique_ptr<Channel> wakeupChannel_;
    Mutex mutex_;

    pid_t currentThreadID_;

    std::unique_ptr<Epoll> epoll_;
};
}
}
#endif
