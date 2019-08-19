#include "Buffer.h"

const int FM_Server::net::Buffer::defaultBufferSize=1024;
const int FM_Server::net::Buffer::defaultPrependSize=8;

FM_Server::net::Buffer::Buffer(int buffersize):
    prependSize_(defaultPrependSize),
    bufferSize_(buffersize),
    buffer_(prependSize_+bufferSize_),
    readIndex_(prependSize_),
    writeIndex_(prependSize_)
{}

FM_Server::net::Buffer::~Buffer(){}

size_t FM_Server::net::Buffer::readFd(int fd)
{
    char extraBuffer[65536];
    memset(extraBuffer,0,sizeof(extraBuffer));
    struct iovec iov[2];
    iov[0].iov_base=Begin()+writeIndex_;
    int writeablebyte=writeableByte();
    iov[0].iov_len=writeablebyte;
    iov[1].iov_base=extraBuffer;
    iov[1].iov_len=sizeof(extraBuffer);
    int len=readv(fd,iov,2);
    if(len<0)
    {
        LOG_ERROR<<"read error in Buffer";
    }
    else if(len<=writeablebyte)
    {
        writeIndex_+=len;
    }
    else
    {
        writeIndex_=buffer_.size();
    }
    return len;
}

void FM_Server::net::Buffer::append(void* data,int len)
{
    if(writeableByte()<len)
    {
        getSpace(len);
    }
    assert(writeableByte()>=len);
    memcpy(writeBegin(),data,len);
    writeIndex_+=len;
}

void FM_Server::net::Buffer::getSpace(int len)
{
    int availByte=writeableByte()+headAvailByte();
    if(availByte<len)
    {
        buffer_.resize(writeIndex_+len);
    }
    else
    {
        int readablebyte=readableByte();
        std::copy(Begin()+readIndex_,Begin()+writeIndex_,Begin()+prependSize_);
        readIndex_=prependSize_;
        writeIndex_=readIndex_+readablebyte;
    }
}

void FM_Server::net::Buffer::retrieve(int len)
{
    if(len<=readableByte())
    {
        readIndex_+=len;
    }
    else
    {
        retrieveAll();
        LOG_WARNING<<"can not retrieve"<<len<<"byte,just retrieve availiable byte";
    }
}

void FM_Server::net::Buffer::retrieveAll()
{
    readIndex_=prependSize_;
    writeIndex_=prependSize_;
}

std::string FM_Server::net::Buffer::retrieveAsString(int len)
{
    assert(len<=readableByte());
    std::string temp(readBegin(),len);
    retrieve(len);
    return temp;
}

std::string FM_Server::net::Buffer::getAsString(int len)
{
    assert(len<=readableByte());
    std::string temp(readBegin(),len);
    return temp;
}
