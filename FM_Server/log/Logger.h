#ifndef LOGGER_H
#define LOGGER_H

#include"LogStream.h"
#include"../base/TimeStamp.h"
#include"../base/CurrentThread.h"
#include<functional>
#include<iostream>
#include<string>
#include<sstream>

namespace FM_Server
{
enum LogLevel
{
    TRACE,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    LOGLEVEL_NUM
};

class Logger
{
    typedef std::function<void (const char* line,int len)> OutputFun;
    public:
        Logger(const char* filename,int line,LogLevel loglevel):implement_(filename,line,loglevel){};
        ~Logger();
        LogStream& stream(){return implement_.logstream_;}
        static void setOutputFun(OutputFun outfun);

        static OutputFun outputFun;
    private:
        //get real log infomation,use this class to encape,avoid user access
        class Implement
        {
            public:
                Implement(const char* filename,int line,LogLevel loglevel);
                ~Implement();
                void finish();
                TimeStamp       timestamp_;
                std::string     FileName_;
                int             line_;
                LogLevel        loglevel_;
                LogStream       logstream_;
                CurrentThread   currenthread_;
        };
        Implement implement_;
};
#define LOG_TRACE FM_Server::Logger(__FILE__,__LINE__,FM_Server::TRACE).stream()
#define LOG_DEBUG FM_Server::Logger(__FILE__,__LINE__,FM_Server::DEBUG).stream()
#define LOG_INFO FM_Server::Logger(__FILE__,__LINE__,FM_Server::INFO).stream()
#define LOG_WARNING FM_Server::Logger(__FILE__,__LINE__,FM_Server::WARNING).stream()
#define LOG_ERROR FM_Server::Logger(__FILE__,__LINE__,FM_Server::ERROR).stream()
}//end namespace ZK_ImageServer
#endif // LOGGER_H

