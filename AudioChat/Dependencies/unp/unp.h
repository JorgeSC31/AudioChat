#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

sockaddr_in SocketAddr(in_port_t t, short port, const char *addr);
sockaddr_in SocketAddrAny(in_port_t family, short port);
void Inet_aton(const char *addr, struct in_addr *sin_addr);

int Socket(int domain, int type, int protocol);
void Connect(int fd, const sockaddr *addr, socklen_t len);
