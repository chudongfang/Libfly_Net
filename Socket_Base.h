/*************************************************************************
	> File Name: Socket_Base.h
	> Author: 
	> Mail: 
	> Created Time: 2018年02月01日 星期四 17时06分20秒
 ************************************************************************/

#ifndef _SOCKET_BASE_H
#define _SOCKET_BASE_H

//封装最低层的网络API

#include <arpa/inet.h>
#include <endian.h>

namespace libfly
{

namespace sockets
{
 
inline uint64_t hostToNetwork64(uint64_t host64)
{
  return htobe64(host64);
}

inline uint32_t hostToNetwork32(uint32_t host32)
{
  return htonl(host32);
}

inline uint16_t hostToNetwork16(uint16_t host16)
{
  return htons(host16);
}

inline uint64_t networkToHost64(uint64_t net64)
{
  return be64toh(net64);
}

inline uint32_t networkToHost32(uint32_t net32)
{
  return ntohl(net32);
}

inline uint16_t networkToHost16(uint16_t net16)
{
  return ntohs(net16);
}






int creNonblockSocket();

void bind(int sockfd , const struct sockaddr_in& addr);

void listen(int sockfd);

int accept(int sockfd , struct sockaddr_in * addr);

void close(int sockfd);

void shutdownWrite(int sockfd);
    
    
void toHostPort(char * buf, size_t size, const struct sockaddr_in& addr);

void fromHostPort(const char * ip, uint16_t port, struct sockaddr_in* addr);


struct sockaddr_in getLocalAddr(int sockfd);






}


}






#endif
