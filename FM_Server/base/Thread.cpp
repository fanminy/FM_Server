#include"Thread.h"
#include<sys/prctl.h>

namespace FM_Server
{
class ThreadData
{
    typedef FM_Server::Thread::ThreadFunc ThreadFunc;
    public:
          ThreadData(const ThreadFunc& func,const std::string& name,pid_t* tid,CountDownLatch* countdownlatch)
              :func_(func),
               name_(name),
               tid_(tid),
               countdownlatch_(countdownlatch)
               {};

          void runThread()
          {
              *tid_=static_cast<pid_t>(syscall(SYS_gettid));
              tid_=NULL;
              countdownlatch_->CountDown();
              countdownlatch_=NULL;

              std::string temp_name=name_.empty()?"FMTHREAD":name_;
              prctl(PR_SET_NAME,temp_name.c_str());

              try
              {
                  func_();
              }
              catch(const std::exception& ex)
              { 
                  temp_name="crashed";
                  fprintf(stderr,"exception caught in Thread %s\n",temp_name.c_str());
                  fprintf(stderr,"reason:%s\n",ex.what());
                  abort();
              }
              catch(...)
              {
                  temp_name="crashed";
                  fprintf(stderr,"unkown exception caught in Thread %s\n",temp_name.c_str());
                  throw;
              }
          }

    private:
          ThreadFunc func_;
          std::string name_;
          pid_t* tid_;
          CountDownLatch* countdownlatch_;
};

void* startThread(void* obj)
{
   ThreadData* data=static_cast<ThreadData*>(obj);
   data->runThread();
   delete data;
   return NULL;
}
}

using namespace FM_Server;
std::atomic<int> Thread::numCreated_(0);
void Thread::setDefaultName()
{
    if(name_.empty())
    {
        char buf[32];
        int num=numCreated_;
        snprintf(buf,sizeof(buf),"Thread%d",num);
        name_=buf;
    }
}

Thread::Thread(const ThreadFunc& func,const std::string& name)
:Isstart_(false),
 Isjoin_(false),
 threadId_(0),
 tid_(0),
 func_(func),
 name_(name),
 countdownlatch_(1)
{
    setDefaultName();
}

Thread::~Thread()
{
    if(Isstart_&&!Isjoin_)
    {
        pthread_detach(threadId_);
    }
}

void Thread::start()
{
    assert(!Isstart_);
    Isstart_=true;
    ThreadData* data=new ThreadData(func_,name_,&tid_,&countdownlatch_);
    if(pthread_create(&threadId_,NULL,startThread,data)==0)
    {
        countdownlatch_.Wait();
        assert(tid_>0);
    }
    else
    {
        Isstart_=false;
        delete data;
    }
}

int Thread::join()
{
    assert(Isstart_);
    assert(!Isjoin_);
    Isjoin_=true;
    return pthread_join(threadId_,NULL);
}
