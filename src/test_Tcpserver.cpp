/*************************************************************************
	> File Name: test_Server.cpp
	> Author: chudongfang
	> Git: https://github.com/chudongfang
	> Created Time: 2018年02月25日 星期日 15时43分25秒
 ************************************************************************/

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

    TcpServer server(2,2);
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.start();
    return 0;
}


