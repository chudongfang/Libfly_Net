/*************************************************************************
	> File Name: InetAddress.h
	> Author: 
	> Mail: 
	> Created Time: 2018年02月01日 星期四 20时10分24秒
 ************************************************************************/

#ifndef _INETADDRESS_H
#define _INETADDRESS_H
#include<string>
#include<netinet/in.h>

namespace libfly
{
    
class NetAddr 
{
public:
    NetAddr(uint16_t port);

    NetAddr(const std::string& ip, uint16_t port);

    NetAddr(const struct sockaddr_in& addr)
        : addr_(addr)
    { }
    
    std::string toHostPort() const ;
    struct sockaddr_in& getAddr() const { return addr_; }
    void setAddr(const struct sockaddr_in& addr) {addr_=  addr;};

private:
    
    struct sockaddr_in addr_;

};



}


#endif
