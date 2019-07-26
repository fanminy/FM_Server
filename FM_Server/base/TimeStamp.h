#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include<string>
#include<time.h>
#include<sys/time.h>

namespace FM_Server
{
class TimeStamp
{
    public:
        TimeStamp():time_string_(),time_(NULL){};
        ~TimeStamp(){};
        std::string gettimeString();
    private:
        std::string time_string_;
        tm*         time_;
};
}
#endif 
