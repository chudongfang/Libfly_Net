/*************************************************************************
	> File Name: Socket.h
	> Author: 
	> Mail: 
	> Created Time: 2018年01月30日 星期二 18时20分54秒
 ************************************************************************/

#ifndef _SOCKET_H
#define _SOCKET_H


namespace libfly
{

class InetAddress;


class Socket 
{
public:
    explicit Socket(int sockfd)
     :sockfd_(sockfd)
    {}
    ~Socket();
    int fd() const   { return sockfd_; }
    void bindAddress(const InetAddress& localaddr);
    void listen();
    //接受
    int accept(InetAddress * peeraddr);
    //关闭Socket
    void shutdownWrite();



private:
    const int sockfd_;

};



}






#endif
