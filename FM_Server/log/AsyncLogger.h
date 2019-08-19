#ifndef ASYNCLOGGER_H
#define ASYNCLOGGER_H

#include "LogStream.h"
#include "../base/Thread.h"
#include "../base/Condition.h"
#include "functional"
#include "../base/CountDownLatch.h"
#include "LogFile.h"
#include "../base/Mutex.h"
#include <vector>
#include <memory>
#include <string>
#include <assert.h>

namespace FM_Server
{

class AsyncLogger
{

    public:
        AsyncLogger(const char* filename,size_t rollsize = 1000000,int flushinterval = 3);
        ~AsyncLogger();
        void threadFun();
        void start();
        void stop();
        void append(const char* logline,int len);
    private:
        typedef FixBuffer<SmallBufferSize> Buffer;
        typedef std::unique_ptr<Buffer> BufferPtr;
        typedef std::vector<BufferPtr> BufferPtrVector;

        std::string     fileName_;
        size_t          rollSize_;
        const int       flushInterval_;
        Mutex           mutex_;
        Thread          thread_;
        Condition       cond_;
        BufferPtr       currentBuf_;
        BufferPtr       nextBuf_;
        BufferPtrVector bufferVector_;
        bool            runningTag_;
};

}//end namespace

#endif // ASYNCLOGGER_H
