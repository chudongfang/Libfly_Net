/*************************************************************************
	> File Name: TcpServer.cpp
	> Author: chudongfang
	> Git: https://github.com/chudongfang
	> Created Time: 2018年02月25日 星期日 10时46分13秒
 ************************************************************************/
#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <vector>
#include <stack>
#include <iostream>
#include "routine.h"
#include "routine.cpp"
#include "Time_heap.h"
#include "Time_heap.cpp"
#include "Poller.h"
#include "Poller.cpp"
#include "Conditional_variable.cpp"
#include "Conditional_variable.h"
#include "Log.h"
#include "Epoll.h"
#include "Epoll.cpp"
#include "Callback.h"
#include "TcpServer.h"


namespace libfly
{

struct ServerTask
{
    Routine * routine;
    int fd;
    TcpServer* tcpServer_;
};
static std::stack<ServerTask*> g_readwrite;
static int g_listen_fd = -1;









//设置非阻塞Socket
static int SetNonBlock(int iSock)
{
    int iFlags;

    iFlags = fcntl(iSock, F_GETFL, 0);
    iFlags |= O_NONBLOCK;
    iFlags |= O_NDELAY;
    int ret = fcntl(iSock, F_SETFL, iFlags);
    return ret;
}




//设置地址
static void SetAddr(const char *pszIP,const unsigned short shPort,struct sockaddr_in &addr)
{
	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(shPort);
	int nIP = 0;
	if( !pszIP || '\0' == *pszIP   
	    || 0 == strcmp(pszIP,"0") || 0 == strcmp(pszIP,"0.0.0.0") 
		|| 0 == strcmp(pszIP,"*") 
	  )
	{
		nIP = htonl(INADDR_ANY);
	}
	else
	{
		nIP = inet_addr(pszIP);
	}
	addr.sin_addr.s_addr = nIP;

}

//创建socket
static int CreateTcpSocket(const unsigned short shPort  = 0 ,const char *pszIP  = "*" ,bool bReuse  = false )
{
	int fd = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);
	if( fd >= 0 )
	{
		if(shPort != 0)
		{
			if(bReuse)
			{
				int nReuseAddr = 1;
				setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&nReuseAddr,sizeof(nReuseAddr));
			}
			struct sockaddr_in addr ;
			SetAddr(pszIP,shPort,addr);
			int ret = bind(fd,(struct sockaddr*)&addr,sizeof(addr));
			if( ret != 0)
			{
				close(fd);
				return -1;
			}
		}
	}
	return fd;
}

static void * Server_ReadWrite_Func(void *arg)
{
    ServerTask *tsk = (ServerTask*) arg;
    char buf[1024*10];
    for(;;)
    {
        //当本协程没有监听Socket是，把其放入协程等待队列
        if(tsk->fd == -1)
        {
            g_readwrite.push(tsk);
            get_curr_routine()->yield();
        }
    
        //设置为-1表示已读，方便协程下次循环退出
        int fd = tsk->fd;
        tsk->fd = -1;
        

        struct epoll_event revents[1000];
        //不断监听
        //不断循环等待，等待读取信息
        for(;;)
        {
            struct epoll_event env;
            env.data.fd = fd;
			env.events = (EPOLLIN|EPOLLERR|EPOLLHUP|EPOLLET);
            int epollret = get_curr_thread_env()->epoll_->addEpoll(&env,1,revents,10);
            if(epollret == 0)
            {
                continue;
            }
            
            if(tsk->tcpServer_->messageCallback_)
            {
                tsk->tcpServer_->messageCallback_(NULL,NULL);
            }

            int ret = read(fd,buf,sizeof(buf));
            if( ret > 0 )
			{
                std::cout<<__FUNCTION__<<__LINE__<<"ret : "<<ret<<std::endl;
                std::cout<<__FUNCTION__<<__LINE__<<"message :: "<<buf<<std::endl;
				//ret = write( fd,buf,ret );
            }
            else
            {
                close(fd);
                break;
            }
			 
        }
    }
}



void * TcpServer::Server_Accept_Func(void *arg)
{
    ServerTask * accepter_tsk = (ServerTask*)arg;
    for(;;)
    {
        std::cout<<"accepter ::" << std::endl;
        if(g_readwrite.empty())
        {
            std::cout<<"There is no routine to read socket! "<<std::endl;
            struct epoll_event  ev;
            ev.data.fd = -1;
			ev.events = (EPOLLIN|EPOLLERR|EPOLLHUP|EPOLLET);
            struct epoll_event revents[10];
            int epollret = get_curr_thread_env()->epoll_->addEpoll(&ev,1,revents,10);
            continue; 
        }
        
        struct sockaddr_in addr; //maybe sockaddr_un;
		memset( &addr,0,sizeof(addr) );
		socklen_t len = sizeof(addr);
        int fd = accept(g_listen_fd, (struct sockaddr *)&addr, &len);
        if(accepter_tsk->tcpServer_->connectionCallback_)
        {
            accepter_tsk->tcpServer_->connectionCallback_(NULL);
        }

        if( fd < 0 )
		{
			struct epoll_event ev;
			ev.data.fd = g_listen_fd;
			ev.events = (EPOLLIN|EPOLLERR|EPOLLHUP|EPOLLET);
            struct epoll_event revents[10];
            int epollret = get_curr_thread_env()->epoll_->addEpoll(&ev, 1, revents, 10);
			continue;
		}

        

		if( g_readwrite.empty() )
		{
			close( fd );
			continue;
		}

		SetNonBlock( fd );
        std::cout<<__FUNCTION__<<" a New Connection !"<<std::endl; 
        //accept完后启动其他协程
        
        //取出最顶端的空闲协程执行对应的fd读写
        ServerTask *tsk = g_readwrite.top();
		tsk->fd = fd;
		tsk->routine->resume();
		g_readwrite.pop();
    }
}


TcpServer::TcpServer(unsigned long long processNum, unsigned long long threadNum,
             unsigned long long routineNum)
 :  processNum_(processNum),
    threadNum_(threadNum),
    routineNum_(routineNum)
{
    
}


//开始Server;
void TcpServer::start()
{
    
     
    int portnumber = 12349;
    char *local_addr="127.0.0.1";
    
    g_listen_fd = CreateTcpSocket(portnumber , local_addr , true);
    listen( g_listen_fd,1024 );
    
    SetNonBlock(g_listen_fd); 
     



    //begin thread routine and process 
	for(int i=0;i<threadNum_;i++)
	{
		ServerTask * tsk = (ServerTask*)calloc( 1,sizeof(ServerTask) );
		tsk->fd = -1;
        tsk->tcpServer_ = this;
        RoutineArr_.push_back(new Routine(get_curr_thread_env(),NULL,Server_ReadWrite_Func,tsk) );	
        tsk->routine = RoutineArr_[i];
	}

    for(int i = 0;i < threadNum_ ;i++)
    {
        RoutineArr_[i]->resume();
    }
    
    
	ServerTask * accepter_tsk = (ServerTask*)calloc( 1,sizeof(ServerTask) );
    accepter_tsk->tcpServer_=this;
    Routine*  accepter = (new Routine(get_curr_thread_env(),NULL,Server_Accept_Func,
                          (void*)(accepter_tsk) ) );	
    accepter->resume();

    EventLoop eventloop(get_curr_thread_env()->time_heap_,NULL,NULL);
    eventloop.loop(); 
}


}



