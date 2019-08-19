#include<stdio.h>
#include<arpa/inet.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<sys/types.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <pthread.h>
#include <stdlib.h>
#include <sys/epoll.h>
using namespace std;

int estabconn()
{  int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        perror("Socket error");
        exit(1);
    }
    struct sockaddr_in serv;
    memset(&serv,0,sizeof(serv));
    serv.sin_family=AF_INET;
    serv.sin_port=htons(8000);//端口
    inet_pton(AF_INET,"192.168.31.106",&serv.sin_addr.s_addr);

    if(connect(sockfd,(struct sockaddr*)&serv,sizeof(serv))<0)
    {
       perror("connect err");
    }
    return sockfd;
}
void writefun(int sockfd)
{
    //通信
    char buf[128]="hello world";
    write(sockfd,buf,strlen(buf));
//    printf("%s\n",buf);
}
void readfun(int epfd,int sockfd)
{
    char buf[128];
    int len=read(sockfd,buf,sizeof(buf));
    if(len==-1)
    {
        perror("read error");
        close(sockfd);
    }
   /* else if(len==0)
    {
        printf("服务器关闭\n");
    }
    else
    {
        printf("recv buf:%s\n",buf);
    }*/
  //   epoll_ctl(epfd,EPOLL_CTL_DEL,sockfd,NULL);
//     close(sockfd);
}
void* threadfun(void* arg)
{
    int num=2500;
    vector<int> sockfds;
    for(int i=0;i<num;i++)
    {
        int tmp=estabconn();
        sockfds.push_back(tmp);
   }
    
    int epfd=epoll_create(1);
    if(epfd<0)
    { 
       perror("epoll create err");
       exit(1);
    }
    epoll_event ev;
    epoll_event ev_ret[2500];
    for(int j=0;j<num;j++)
    {
       memset(&ev,0,sizeof(ev));
       ev.events=EPOLLIN;
       ev.data.fd=sockfds[j];
       if(epoll_ctl(epfd,EPOLL_CTL_ADD,sockfds[j],&ev)!=0)
       {
           perror("epoll ctl err");
           exit(1);
       }
    }
    for(int m=0;m<2500;m++)
    {
        writefun(sockfds[m]);
    }
    while(1)
    {
        int n = 0;
       // printf("before epoll_wait\n");
        int nfds=epoll_wait(epfd,ev_ret,2500,-1);
        if(nfds<0)
        {
         perror("epoll_wait err");
         exit(1);
        }
        for(int k=0;k<nfds;++k)
        {
          int sockfd=ev_ret[k].data.fd;
          if(ev_ret[k].events&EPOLLIN) 
          {
             readfun(epfd,sockfd);
          } 
        }
        //n += nfds;
        //if(n == 10)
          //  break;
    } 
}

int main()
{
    pthread_t ids[4];
    for(int i=0;i<4;i++)
    {
        int ret=pthread_create(&ids[i],NULL,threadfun,NULL);
        if(ret)
        {
         //   cout<<"Create pthread error"<<endl;
            return 1;
        }
    }
    while(1);
}
