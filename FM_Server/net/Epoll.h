#ifndef EHPOLL_H  
#define EHPOLL_H

//#include "EventLoop.h"
//#include "Channel.h"
#include <vector>
#include <map>
#include <assert.h>
#include <sys/epoll.h>
#include <string>
#include <errno.h>
#include "../log/Logger.h"

namespace FM_Server
{
namespace net
{
class EventLoop;
class Channel;
class Epoll
{
    typedef std::vector<Channel*> ChannelList;
    typedef std::map<int,Channel*> ChannelMap;
    public:
        Epoll(EventLoop* ownerLoop);
        ~Epoll();
        void updateChannel(Channel* channel);
        void update(int operation,Channel* channel);
        void removeChannel(Channel* channel);
        void process(int timeoutMs,ChannelList* activeChannel);
        void fillActiveChannel(int eventNum,ChannelList* activeChannel);
        const char* operationToString(int operation);

    private:
        int epollfd_;
        std::vector<struct epoll_event> epoll_events_;
        ChannelMap channelMap_;
        EventLoop* ownerLoop_;
        static const int minEpollEventSize=16;
};
}
}
#endif
