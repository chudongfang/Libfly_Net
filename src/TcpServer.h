/*************************************************************************
	> File Name: TcpServer.h
	> Author: chudongfang
	> Git: https://github.com/chudongfang
	> Created Time: 2018年02月25日 星期日 10时46分22秒
 ************************************************************************/
#ifndef _TCPSERVER_H
#define _TCPSERVER_H
#include<vector>
#include<routine.h>
#include<string>
#include<Callback.h>
namespace libfly
{


class TcpServer
{
public:
    TcpServer(unsigned long long processNum, unsigned long long routineNum);
    ~TcpServer(){};
   
    void start();
    
    void setConnectionCallback(const ConnectionCallback& Callback)
    {
        connectionCallback_ = Callback;
    }
    
    void setMessageCallback(const MessageCallback& Callback)
    {
        messageCallback_ = Callback;
    }

private:   
    static void * Server_Accept_Func(void *);
    static void * Server_ReadWrite_Func(void *arg); 


//FIXME
public:
    unsigned long long processNum_ ;
    unsigned long long threadNum_  ;
    unsigned long long routineNum_ ;
    
    const std::string name_; 
    ConnectionCallback connectionCallback_;
    MessageCallback    messageCallback_; 
    bool started_;
    int ServerSocket_;
    std::vector<Routine*> RoutineArr_;
};





}















#endif
