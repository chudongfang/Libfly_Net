# Libfly_Net
    
    Libfly_Net是一个基于libfly协程库的网络库，其在协程库的基础上添加了Epoll模块，对一些网络基础函数进行了封装。
    
    其为一个多进程/线程-协程模式工作的网络库。
    
    Libfly_Net is a net library based on libfly routine library. 
    
    it adds epoll module to libfly.
    




## How to use/使用方法



```c++
//create a server 
TcpServer server(2,2);

//set ConnetionCallback
server.setConnectionCallback(onConnection);

//set MessageCallback
server.setMessageCallback(onMessage);

//start the server
server.start();
```






the complete file 
```c++
#include"TcpServer.cpp"
#include"TcpServer.h"

using namespace libfly;



void onConnection(void *)
{
    //a new connection comming!
    std::cout<<"A new connection!"<<std::endl;
}


void onMessage(const char* data,
               ssize_t len)
{
    //a new message comming!
    std::cout<<"A new message!"<<std::endl;
}


int main()
{

    //processnumb , routinenumb
    TcpServer server(2,2);
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.start();
    return 0;
}


```


