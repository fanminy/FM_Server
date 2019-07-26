#ifndef CONDITION_H
#define CONDITION_H

#include"Mutex.h"
#include<sys/time.h>
#include<errno.h>
#include<pthread.h>

namespace FM_Server
{
class Condition
{
    public:
        explicit Condition (Mutex& mutex):mutex_(mutex)
        {
            assert(pthread_cond_init(&cond_,NULL)==0);
        }
        ~Condition()
        {
            assert(pthread_cond_destroy(&cond_)==0);
        }

        void wait()
        {
            assert(pthread_cond_wait(&cond_,mutex_.getmutex())==0);
        }

        void notifyone()
        {
            assert(pthread_cond_signal(&cond_)==0);
        }
        void notifyall()
        {
            assert(pthread_cond_broadcast(&cond_)==0);
        }

        void waitForSeconds(int seconds);


    private:
        pthread_cond_t cond_;
        Mutex&         mutex_;
};
}
#endif 
