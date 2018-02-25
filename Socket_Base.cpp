#include "Socket_Base.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>  
#include <strings.h>  
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>



namespace libfly
{

typedef struct sockaddr SA;


void setNonBlockAndCloseOnExec(int sockfd)
{
  // non-block
    int flags = ::fcntl(sockfd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    int ret = ::fcntl(sockfd, F_SETFL, flags);
  // FIXME check

  // close-on-exec
    flags = ::fcntl(sockfd, F_GETFD, 0);
    flags |= FD_CLOEXEC;
    ret = ::fcntl(sockfd, F_SETFD, flags);
  // FIXME check
}


int sockets::creNonblockSocket()
{
    int sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
    {
        std::cout << "sockets::createNonblockingOrDie failed" << std::endl;
    }

    setNonBlockAndCloseOnExec(sockfd);
    return sockfd;
}

void sockets::bind(int sockfd, const struct sockaddr_in& addr)
{
    int ret = ::bind(sockfd,  (struct sockaddr *)&addr, sizeof addr);
    if (ret < 0)
    {
         std::cout << "sockets::bindOrDie failed" << std::endl;
    }
}

void sockets::listen(int sockfd)
{
    int ret = ::listen(sockfd, SOMAXCONN);
    if (ret < 0)
    {
         std::cout<< "sockets::listenOrDie failed" << std::endl;
    }
}

int sockets::accept(int sockfd, struct sockaddr_in* addr)
{
    socklen_t addrlen = sizeof *addr;
    int connfd = ::accept(sockfd,  (struct sockaddr *)addr, &addrlen);
    setNonBlockAndCloseOnExec(connfd);
    if (connfd < 0)
    {
        int savedErrno = errno;
        std::cout << "Socket::accept failed"<<std::endl;
    }
    return connfd;
}

void sockets::close(int sockfd)
{
    if (::close(sockfd) < 0)
    {
        std::cout << "sockets::close failed"<<std::endl;
    }
}


void sockets::shutdownWrite(int sockfd)
{
    if(::shutdown(sockfd,SHUT_WR) < 0 )
    {
        std::cout<<"sockets:: shutdownWrite "<<std::endl;
    }

}


void sockets::toHostPort(char* buf, size_t size,
                         const struct sockaddr_in& addr)
{
  char host[INET_ADDRSTRLEN] = "INVALID";
  ::inet_ntop(AF_INET, &addr.sin_addr, host, sizeof host);
  uint16_t port = sockets::networkToHost16(addr.sin_port);
  snprintf(buf, size, "%s:%u", host, port);
}


void sockets::fromHostPort(const char* ip, uint16_t port,
                           struct sockaddr_in* addr)
{
  addr->sin_family = AF_INET;
  addr->sin_port = hostToNetwork16(port);
  if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
  {
    std::cout << " errors: sockets::fromHostPort"<<std::endl;
  }
}


struct sockaddr_in sockets::getLocalAddr(int sockfd)
{
  struct sockaddr_in localaddr;
  bzero(&localaddr, sizeof localaddr);
  socklen_t addrlen = sizeof(localaddr);
  if (::getsockname(sockfd,  (struct sockaddr *)(&localaddr), &addrlen) < 0)
  {
      // << "sockets::getLocalAddr";
  }
  return localaddr;
}


}
