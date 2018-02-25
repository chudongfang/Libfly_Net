/*************************************************************************
	> File Name: InetAddress.cc
	> Author: 
	> Mail: 
	> Created Time: 2018年02月01日 星期四 20时17分46秒
 ************************************************************************/
#include "NetAddr.h"

#include "Socket_Base.h"

#include <strings.h>  // bzero
#include <netinet/in.h>

namespace libfly
{

static const in_addr_t kInaddrAny  = INADDR_ANY;

NetAddr::NetAddr(uint16_t port)
{
    bzero(&addr_, sizeof addr_);
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = htonl(kInaddrAny);
    addr_.sin_port = htonl(port);
}

NetAddr::NetAddr(const std::string& ip, uint16_t port)
{
    bzero(&addr_, sizeof addr_);
    sockets::fromHostPort(ip.c_str(), port,&addr_);
}

std::string NetAddr::toHostPort() const
{
    char buff[32];
    sockets::toHostPort(buff,sizeof buff, addr_);
    return buff;
}


}
