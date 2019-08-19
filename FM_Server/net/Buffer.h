#ifndef BUFFER_H
#define BUFFER_H

#include<vector>
#include<unistd.h>
#include<string.h>
#include<string>
#include<sys/uio.h>
#include<assert.h>
#include"../log/Logger.h"

namespace  FM_Server
{
namespace net
{
class Buffer
{
    public:
        Buffer(int buffersize=defaultBufferSize);
        ~Buffer();
        size_t readFd(int fd);
        char* Begin(){return &*buffer_.begin();}
        char* writeBegin(){return Begin()+writeIndex_;}
        char* readBegin(){return Begin()+readIndex_;}
        int writeableByte(){return buffer_.size()-writeIndex_;}
        int readableByte(){return writeIndex_-readIndex_;}
        int headAvailByte(){return readIndex_;}
        void append(void* data,int len);
        void getSpace(int len);
        void retrieve(int len);
        void retrieveAll();
        std::string retrieveAsString(int len);
        std::string getAsString(int len);
    private:
        size_t prependSize_;
        size_t bufferSize_;
        std::vector<char> buffer_;
        size_t readIndex_;
        size_t writeIndex_;

        static const int defaultBufferSize;
        static const int defaultPrependSize;
};
}
}


#endif 
