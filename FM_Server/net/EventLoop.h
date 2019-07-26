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

}
}
}
#endif
