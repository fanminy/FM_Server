#include"Condition.h"

void FM_Server::Condition::waitForSeconds(int seconds)
{
    struct timeval now;
    struct timespec tsp;
    gettimeofday(&now,NULL);
    tsp.tv_sec=now.tv_sec+seconds;
    tsp.tv_nsec=now.tv_usec*1000;/*usec to nsec*/
    assert(pthread_cond_timedwait(&cond_,mutex_.getmutex(),&tsp)==0);

}

