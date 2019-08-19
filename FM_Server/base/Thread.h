#ifndef THREAD_H
#define THREAD_H

#include<iostream>
#include<stdio.h>
#include<string>
#include<pthread.h>
#include<assert.h>
#include<functional>
#include<atomic>
#include<sys/prctl.h>
#include "Mutex.h"
#include"CountDownLatch.h"

namespace FM_Server
{
class Thread
{
public:
    typedef std::function<void()> ThreadFunc;

    explicit Thread(const ThreadFunc& func,const std::string& name=std::string());
    ~Thread();

    void start();
    int join();
    bool Isstart() const {return Isstart_;}
    pid_t gettid() const {return tid_;}
    std::string getname() const {return name_;}


private:
    void setDefaultName();

    bool Isstart_;
    bool Isjoin_;
    pthread_t threadId_;
    pid_t tid_;
    ThreadFunc func_;
    std::string name_;
    CountDownLatch countdownlatch_;
    static std::atomic<int> numCreated_;


};
}

#endif 
