/*************************************************************************
	> File Name: test_Server.cpp
	> Author: chudongfang
	> Git: https://github.com/chudongfang
	> Created Time: 2018年02月25日 星期日 15时43分25秒
 ************************************************************************/

#include"./TcpServer.cpp"
#include"./TcpServer.h"

using namespace libfly;


int main()
{
    TcpServer Test(100,100,100);
    Test.start();
    return 0;
}


