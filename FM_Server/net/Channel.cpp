#include "Channel.h"

const int FM_Server::net::Channel::NoneEvent=0;
const int FM_Server::net::Channel::ReadEvent=EPOLLIN;
const int FM_Server::net::Channel::WriteEvent=EPOLLOUT;

FM_Server::net::Channel::Channel(EventLoop* loop,int fd):
    event_(0),
    revent_(0),
    fd_(fd),
    isEventHanding_(false),
    index_(-1),
    isAddedToLOOP_(false),
    ownerLoop_(loop),
    readCB_(NULL),
    writeCB_(NULL),
    closeCB_(NULL),
    errorCB_(NULL),
    tied_(false)
    {}

FM_Server::net::Channel::~Channel(){}


void FM_Server::net::Channel::update()
{
   isAddedToLOOP_=true;
   ownerLoop_->updateChannel(this);//这里相当于把channel传递进去
}

void FM_Server::net::Channel::removeFromLoop()
{
    isAddedToLOOP_=false;
    assert(event_==NoneEvent);
    ownerLoop_->removeChannel(this);
}

void FM_Server::net::Channel::handleEvent()
{
    isEventHanding_=true;

    if(revent_&EPOLLIN)
    {
       LOG_TRACE<<"call read callback";
       if(readCB_)
       {
           LOG_TRACE<<"there is a readcallback,call it";
           readCB_();
       }
    }

    if(revent_&EPOLLOUT)
    {
      if(writeCB_)
      {
          writeCB_();
      }
    }

    if(revent_&EPOLLERR)
    {
        if(errorCB_)
        {
            errorCB_();
        }
        LOG_TRACE<<"error happen in EPOLL event";
    }
    isEventHanding_=false;
}


const char* FM_Server::net::Channel::getEventString()
{
    switch(event_)
    {
        case 0:
            return "NoneEvent";
        case EPOLLIN:
            return "ReadEvent";
        case EPOLLOUT:
            return "WriteEvent";
        case EPOLLHUP:
            return "HubEvent";
        default:
            return "unkown event";
    }
}

