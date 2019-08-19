#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <memory>
#include <map>
#include <functional>
#include "Acceptor.h"
#include "EventLoop.h"
#include "EventLoopThreadPool.h"
#include "TcpConnection.h"
#include "../log/Logger.h"

namespace FM_Server
{
namespace net
{
class TcpServer
{
    typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
    typedef std::function<void(TcpConnectionPtr)> CallBackFun;
    typedef std::function<void(TcpConnectionPtr,Buffer*)> MessageCallBackFun;
    typedef std::map<int,TcpConnectionPtr> ConnectionMap;

    public:
    TcpServer(CallBackFun newConnCB,MessageCallBackFun messageCB,int threadNum,EventLoop* baseLoop);
    ~TcpServer();
    void newConnection(int connfd);
    void destoryConnection(TcpConnectionPtr conn);
    void setwriteCompleteCB(CallBackFun writeCompleteCB){writeCompleteCB_=writeCompleteCB;}
    private:
    EventLoop*             baseLoop_;
    CallBackFun            newConnCB_;
    MessageCallBackFun     messageCB_;
    CallBackFun            writeCompleteCB_;
    std::unique_ptr<Acceptor>   acceptor_;
    std::unique_ptr<EventLoopThreadPool>   eventLoopThreadPool_;
    ConnectionMap          connectionMap_;
};
}
}

#endif 
