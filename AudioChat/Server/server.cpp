#include <bits/stdc++.h>
#include <broadcast.h>
#include <config.h>
#include <stdio.h>
#include <string.h>
#include <unp.h>

int main() {
    // Createing server socket
    int sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in servAddr = SocketAddrAny(AF_INET, SERVER_PORT);
    Bind(sockfd, servAddr);

    // Client address, needed for reply
    sockaddr_in cliAddr;
    socklen_t lenCliAddr;
    mZero(&cliAddr, sizeof(cliAddr));

    // Buffer message
    char* buffer = new char[BUFFER_SIZE];

    // Server cycle
    Broadcast broadcast;
    while (true) {
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, MSG_WAITALL,
                         (SA*)&cliAddr, &lenCliAddr);
        broadcast.addDest(cliAddr);
        broadcast.sendFrom(cliAddr, sockfd, buffer, n);
    }

    return 0;
}
