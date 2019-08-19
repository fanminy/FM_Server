#ifndef APPENDFILE_H
#define APPENDFILE_H
#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
namespace FM_Server
{

class AppendFile
{
    public:
        AppendFile(const char* filename);
        ~AppendFile();
        void append(const char* line, int len);
        void flush();
        size_t getWritenBytes(){return writenBytes_;}
        void setWritenBytes(size_t value){writenBytes_ = value;}
    protected:
    private:
        FILE* fp_;
        char  buffer_[64*1024];
        size_t writenBytes_;


};
}//end namespace FM_Server
#endif // APPENDFILE_H
