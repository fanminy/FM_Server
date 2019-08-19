#ifndef CHANNEL_H
#define CHANNEL_H

#include<functional>
#include<sys/epoll.h>
#include<memory>
#include<string>
//#include"EventLoop.h"
#include "../log/Logger.h"
#include <assert.h>
namespace FM_Server
{
namespace net
{
class EventLoop;

class Channel
{
    typedef std::function<void()> CallBackFun;
    public:
    Channel(EventLoop* loop,int fd);
    ~Channel();

    void setReadCallBack(CallBackFun cb){readCB_=cb;}
    void setWriteCallBack(CallBackFun cb){writeCB_=cb;}
    void setCloseCallBack(CallBackFun cb){closeCB_=cb;}
    void setErrorCallBack(CallBackFun cb){errorCB_=cb;}

    void update();//认为应该写在private里面，因为用户用不到
    void removeFromLoop();
    void enableRead(){event_=ReadEvent;update();}
    void enableWrite(){event_=WriteEvent;update();}
    void setRevent(int ev){revent_=ev;}
    void disableWriting(){event_&=~WriteEvent;}
    void disableAll(){event_=NoneEvent;update();}

    void handleEvent();

    EventLoop* getOwnerLoop(){return ownerLoop_;}
    void setIndex(int index){index_=index;}
    int getIndex(){return index_;}
    int getFd(){return fd_;}
    int getEvent(){return event_;}
    bool isNoneEvent(){return event_==NoneEvent;}
    bool isWriting(){return event_==WriteEvent;}
    bool isReading(){return event_==ReadEvent;}

    const char* getEventString();

    void tie(std::shared_ptr<void>& obj){tie_=obj;tied_=true;}

    private:
    int event_;//现在的状态
    int revent_;
    int fd_;
    bool isEventHanding_;
    int index_;

    bool isAddedToLOOP_;

    static const int NoneEvent;
    static const int ReadEvent;
    static const int WriteEvent;

    EventLoop* ownerLoop_;

    CallBackFun readCB_;
    CallBackFun writeCB_;
     CallBackFun closeCB_;
     CallBackFun errorCB_;

     std::weak_ptr<void> tie_;
     bool tied_;

};
}
}
#endif
