#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define SA struct sockaddr

sockaddr_in SocketAddr(short family, short port, const char *addr);
sockaddr_in SocketAddrAny(short family, short port);
void Inet_aton(const char *addr, struct in_addr *sin_addr);

int Socket(int domain, int type, int protocol);
void Connect(int fd, const sockaddr *addr, socklen_t len);
void Bind(int socketfd, const sockaddr_in &sockAddr);
void Listen(int fd, int backlog);