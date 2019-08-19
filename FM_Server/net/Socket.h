#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include "../log/Logger.h"

namespace FM_Server
{
namespace net
{
class Socket
{
    public:
        Socket(int listenPort=defaultListenPort);
        ~Socket();
        int createSocket();
        void setNonBlock(int fd);
        void bindAndListen();
        void setListenPort(int listenPort){listenPort_=listenPort;};
        int getAcceptSocket();
        int getListenfd(){return listenfd_;}
        const char* getClientAddressString(){
            if(clientLen_>0)
            {
                return inet_ntoa(clientAddr_.sin_addr);
            }
            return NULL;
        }
    private:
        int listenfd_;
        int listenPort_;
        static const int defaultListenPort;

        struct sockaddr_in serverAddr_;
        struct sockaddr_in clientAddr_;
        socklen_t clientLen_;

};
}
}
#endif 
