#include "unp.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

sockaddr_in SocketAddr(in_port_t family, short port, const char *addr) {
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = family;
    sockAddr.sin_port = htons(port);
    Inet_aton(addr, &sockAddr.sin_addr);
    return sockAddr;
}

sockaddr_in SocketAddrAny(in_port_t family, short port) {
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = family;
    sockAddr.sin_port = htons(port);
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    return sockAddr;
}

int Socket(int domain, int type, int protocol) {
    int sockfd = socket(domain, type, protocol);
    if (sockfd < 0) {
        fprintf(stderr, "Error in Socket: %s\n", strerror(errno));
        exit(1);
    }
    return sockfd;
}

void Connect(int fd, const sockaddr *addr, socklen_t len) {
    if (connect(fd, addr, len) < 0) {
        printf("ERROR Connect: %s\n", strerror(errno));
        exit(1);
    }
}

void Inet_aton(const char *addr, struct in_addr *sin_addr) {
    if (!inet_aton(addr, sin_addr)) {
        printf("ERROR Inet_aton: %s\n", strerror(errno));
        exit(1);
    }
}