#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <memory>
#include <functional>
#include <sys/socket.h>
#include "EventLoop.h"
#include "Channel.h"
#include "Buffer.h"
namespace FM_Server
{
namespace net
{
class TcpConnection:public std::enable_shared_from_this<TcpConnection>
{
    enum STATE{Disconnected,Disconnecting,Connecting,Connected};
    public:
    typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
    typedef std::function<void(TcpConnectionPtr)> CallBackFun;
    typedef std::function<void(TcpConnectionPtr,Buffer*)> MessageCallBackFun;

    TcpConnection(EventLoop* ownerLoop_,int fd);
    ~TcpConnection();
    void handleRead();
    void handleWrite();
    void handleClose();
    void handleError();

    void setNewConnectionCB(CallBackFun newConnetionCB){newConnectionCB_=newConnetionCB;}
    void setWriteCompleteCB(CallBackFun writeCompleteCB){writeCompleteCB_=writeCompleteCB;}
    void setMessageCallBackFun(MessageCallBackFun messageRecvCB){messageRecvCB_=messageRecvCB;}
    void setCloseCallBackFun(CallBackFun closeCB){closeCB_=closeCB;}

    void connEstablished();
    void connDestroyed();
    
    void sendInLoop(void* data,int len);
    void readInLoop();

    void setState(int state){state_=state;}
    int getConnfd(){return connfd_;}
    EventLoop* getOwnerLoop(){return ownerLoop_;}
    private:
    int                        connfd_;
    int                        state_;
    EventLoop*                 ownerLoop_;
    std::unique_ptr<Channel>   channel_;
    CallBackFun                newConnectionCB_;
    MessageCallBackFun         messageRecvCB_;
    CallBackFun                writeCompleteCB_;
    CallBackFun                closeCB_;
    Buffer                     inputBuffer_;
    Buffer                     outputBuffer_;
};
}
}

#endif 
