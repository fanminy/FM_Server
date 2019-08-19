#ifndef  ACCEPTOR_H
#define  ACCEPTOR_H

#include "Socket.h"
#include "EventLoop.h"
#include "Channel.h"
#include "../log/Logger.h"
#include <functional>
#include <assert.h>

namespace  FM_Server
{
namespace  net
{
class Acceptor
{
    typedef std::function<void(int connfd)> NewConnectionCallBack;
    public:
    Acceptor(EventLoop* ownerLoop_);
    ~Acceptor();
    void handleRead();
    void setNewConnectionCallBack(NewConnectionCallBack cb){newConnectionCallBack_=cb;};
    void startListen();
    void setListenPort(int port){socket_.setListenPort(port);}
    private:
    EventLoop* ownerLoop_;
    Socket     socket_;
    Channel    acceptChannel_;
    bool       isListening_;
    NewConnectionCallBack   newConnectionCallBack_;
};
}
}

#endif 
