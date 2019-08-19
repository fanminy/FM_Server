#include "../FM_Server/log/Logger.h"
#include "../FM_Server/base/TimeStamp.h"
#include "../FM_Server/net/TcpServer.h"
#include "../FM_Server/net/EventLoop.h"
#include "../FM_Server/net/TcpConnection.h"
#include "../FM_Server/net/Buffer.h"
#include "../FM_Server/log/AsyncLogger.h"
#include <unistd.h>

using namespace std;
//using namespace FM_Server;

static int zsum=0;

FM_Server::AsyncLogger g_AsyncLogger("__FILE__",1000000);
const int defaultthreadNum=2;
typedef std::shared_ptr<FM_Server::net::TcpConnection> TcpConnectionPtr;

void newconnCB(TcpConnectionPtr conn)
{
   LOG_TRACE<<"message has arrived,connnectioncallbackfun has been called";
}

void messageCB(TcpConnectionPtr conn,FM_Server::net::Buffer* buffer)
{
   LOG_TRACE<<"messageCB has been called";
   std::string temp=buffer->retrieveAsString(buffer->readableByte());
   conn->sendInLoop(&*temp.begin(),temp.size());
   LOG_TRACE<<"read/write sucess";
   printf("zsum=%d\n",zsum++);
}
void testOutput(const char* line,int len)
{
    g_AsyncLogger.append(line,len);
}
int main()
{
    FM_Server::Logger::setOutputFun(testOutput);
    g_AsyncLogger.start();
    FM_Server::net::EventLoop loop;
    FM_Server::net::TcpServer server(newconnCB,messageCB,defaultthreadNum,&loop);
    loop.loop();
    pthread_exit(0);
}
