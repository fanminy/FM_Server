#include "Acceptor.h"

FM_Server::net::Acceptor::Acceptor(EventLoop* ownerLoop):
ownerLoop_(ownerLoop),
socket_(),
acceptChannel_(ownerLoop_,socket_.getListenfd()),
isListening_(false),
newConnectionCallBack_(NULL)
{
    acceptChannel_.setReadCallBack(std::bind(&Acceptor::handleRead,this));
}

FM_Server::net::Acceptor::~Acceptor()
{
    acceptChannel_.disableAll();
    acceptChannel_.removeFromLoop();
}

void FM_Server::net::Acceptor::startListen()
{
    assert(ownerLoop_->isInLoopThread());
    isListening_=true;
    socket_.bindAndListen();
    acceptChannel_.enableRead();
}

void FM_Server::net::Acceptor::handleRead()
{
    int connfd=socket_.getAcceptSocket();
    if(connfd>=0)
    {
        LOG_INFO<<"new client has connected,address is"<<socket_.getClientAddressString();
        if(newConnectionCallBack_)
        {
            newConnectionCallBack_(connfd);
        }
        else
        {
            close(connfd);
            LOG_INFO<<"no newConnectionCallBack_ in acceptor";
        }
    }
    else
    {
        LOG_ERROR<<"error happen in acceptor handle read";
    }
}

