#ifndef MUTEX_H
#define MUTEX_H

#include<string>
#include<pthread.h>
#include<unistd.h>
#include<sys/syscall.h>
#include<assert.h>
#include"CurrentThread.h"

namespace FM_Server
{
class Mutex
{
public:
    Mutex():currentThread_(),holder_(0) {assert(pthread_mutex_init(&mutex_,NULL)==0);}
    ~Mutex(){assert(holder_==0);assert(pthread_mutex_destroy(&mutex_)==0);}

    void lock()
    {
        assert(pthread_mutex_lock(&mutex_)==0);
        assignholder();
    }
    void unlock()
    {
        assert(pthread_mutex_unlock(&mutex_)==0);
        unassignholder();
    }

    bool IsLockedbyThisThread()
    {
        return holder_==currentThread_.gettid();
    }

    pthread_mutex_t* getmutex()
    {
        return &mutex_;
    }
    private:
    friend class Condition;
    
    class UnassignGuard
    {
    public:
        UnassignGuard(Mutex& mutex):mutex_(mutex)
        {
            mutex_.unassignholder();
        }
        ~UnassignGuard()
        {
            mutex_.assignholder();
        }
    private:
        Mutex& mutex_;
    };

    void assignholder()
    {
        holder_=currentThread_.gettid();
    }

    void unassignholder()
    {
        holder_=0;
    }


    CurrentThread currentThread_;
    pthread_mutex_t mutex_;
    pid_t   holder_;
};

class MutexGuard
{
public:
    explicit MutexGuard(Mutex& mutex):mutex_(mutex)
    {
        mutex_.lock();
    }

    ~MutexGuard()
    {
        mutex_.unlock();
    }
private:
    Mutex& mutex_;
};

}

#endif
