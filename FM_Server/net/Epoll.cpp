#include "Epoll.h"
#include "Channel.h"
//using namespace FM_Server;

const int ChannelNew=-1;
const int ChannelAdded=1;//added into eventloop
const int ChannelDeleted=2;//delete from eventloop

FM_Server::net::Epoll::Epoll(EventLoop* ownerLoop):
    epollfd_(epoll_create1(EPOLL_CLOEXEC)),
    epoll_events_(minEpollEventSize),
    channelMap_(),
    ownerLoop_(ownerLoop)
{}     

FM_Server::net::Epoll::~Epoll(){}

void FM_Server::net::Epoll::updateChannel(Channel* channel)
{
    int index=channel->getIndex();
    int fd=channel->getFd();
    if(index==ChannelNew||index==ChannelDeleted)
    {
        if(index==ChannelNew)
        {
            assert(channelMap_.find(fd)==channelMap_.end());
            channelMap_[fd]=channel;
        }
        else
        {
            assert(channelMap_.find(fd)!=channelMap_.end());
            assert(channelMap_[fd]==channel);
        }
        channel->setIndex(ChannelAdded);
        update(EPOLL_CTL_ADD,channel);
    }
    else if(index==ChannelAdded)
    {
        assert(channelMap_.find(fd)!=channelMap_.end());
        assert(channelMap_[fd]==channel);
        if(channel->isNoneEvent())
        {
            update(EPOLL_CTL_DEL,channel);
            channel->setIndex(ChannelDeleted);
        }
        else
        {
            update(EPOLL_CTL_MOD,channel);
        }
    }
}

void FM_Server::net::Epoll::removeChannel(Channel* channel)
{
    int index=channel->getIndex();
    int fd=channel->getFd();
    assert(index==ChannelAdded);
    assert(channelMap_.find(fd)!=channelMap_.end());
    assert(channelMap_[fd]==channel);
    assert(channel->isNoneEvent());
    int n=channelMap_.erase(fd);
    assert(n==1);
    update(EPOLL_CTL_DEL,channel);
    channel->setIndex(ChannelNew);
}

void FM_Server::net::Epoll::update(int operation,Channel* channel)
{
    struct epoll_event event;
    event.events=channel->getEvent();
    event.data.ptr=channel;
    int fd=channel->getFd();
    LOG_INFO<<"epoll operation"<<operationToString(operation)<<"of fd:"<<fd<<channel->getEventString();
    if(epoll_ctl(epollfd_,operation,fd,&event)<0)
    {
        LOG_ERROR<<"epoll ctl error,operation is"<<operationToString(operation)<<"fd is"<<fd;
    }
}

void FM_Server::net::Epoll::process(int timeOutMs,ChannelList* activeChannel)
{
    int eventNum=epoll_wait(epollfd_,&*epoll_events_.begin(),static_cast<int>(epoll_events_.size()),timeOutMs);
    if(eventNum==0)
    {

    }
    else if(eventNum>0)
    {
        fillActiveChannel(eventNum,activeChannel);
        if(eventNum==static_cast<int>(epoll_events_.size()))
        {
            epoll_events_.resize(epoll_events_.size()*2);
        }
    }
    else 
    {
        int errNUM=errno;
        LOG_ERROR<<"Error happen in epoll,eventNum is"<<eventNum<<"errno is"<<errNUM;
    }
}

void FM_Server::net::Epoll::fillActiveChannel(int eventNum,ChannelList* activeChannel)
{
    for(int i=0;i<eventNum;i++)
    {
        Channel* channel=static_cast<Channel*>(epoll_events_[i].data.ptr);
        int fd=channel->getFd();
        assert(channelMap_.find(fd)!=channelMap_.end());
        channel->setRevent(channel->getEvent());
        activeChannel->push_back(channel);
    }
}

const char* FM_Server::net::Epoll::operationToString(int operation)
{
    switch(operation)
    {
        case EPOLL_CTL_ADD:
            return "EPOLL ADD";
        case EPOLL_CTL_MOD:
            return "EPOLL MOD";
        case EPOLL_CTL_DEL:
            return "EPOLL EDL";
        default:
            return "Unknow operation";
    }
}
