#include "TcpServer.h"

FM_Server::net::TcpServer::TcpServer(CallBackFun newConnCB,
                                     MessageCallBackFun messageCB,int threadNum,EventLoop* baseLoop):
baseLoop_(baseLoop),
newConnCB_(newConnCB),
messageCB_(messageCB),
acceptor_(new Acceptor(baseLoop_)),
eventLoopThreadPool_(new EventLoopThreadPool(baseLoop_,threadNum)),
connectionMap_()
{
    acceptor_->setNewConnectionCallBack(std::bind(&TcpServer::newConnection,this,std::placeholders::_1));
    eventLoopThreadPool_->start();
    baseLoop_->runInLoop(std::bind(&Acceptor::startListen,&*acceptor_));
    LOG_TRACE<<"Tcp Server has established,start listen";
}

FM_Server::net::TcpServer::~TcpServer()
{
    LOG_INFO<<"TCP server stop";
}

void FM_Server::net::TcpServer::newConnection(int connfd)
{
    EventLoop* ioLoop=eventLoopThreadPool_->getNextLoop();
    if(ioLoop==baseLoop_)
    {
        LOG_INFO<<"eventLoopThreadPool_ don't work";
    //    ioLoop=eventLoopThreadPool_->getNextLoop();
    }
    LOG_INFO<<"new connection is estalished,connfd is"<<connfd;
    TcpConnectionPtr tcpConnectionPtr(new TcpConnection(ioLoop,connfd));
    connectionMap_[connfd]=tcpConnectionPtr;
    tcpConnectionPtr->setNewConnectionCB(newConnCB_);
    tcpConnectionPtr->setMessageCallBackFun(messageCB_);
    tcpConnectionPtr->setCloseCallBackFun(std::bind(&TcpServer::destoryConnection,this,std::placeholders::_1));
    ioLoop->runInLoop(std::bind(&TcpConnection::connEstablished,tcpConnectionPtr));
}

void FM_Server::net::TcpServer::destoryConnection(TcpConnectionPtr conn)
{
    int fd=conn->getConnfd();
    LOG_INFO<<"remove connection from TcpServer,connection fd is"<<fd;
    int n=connectionMap_.erase(fd);
    assert(n==1);
    EventLoop* ioLoop=conn->getOwnerLoop();
    ioLoop->queueInLoop(std::bind(&TcpConnection::connDestroyed,conn));
}
