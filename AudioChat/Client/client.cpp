#include <string.h>
#include <unistd.h>
#include <unp.h>

#include <iostream>

#define PORT 8080
#define MAXLINE 100

int main() {
    int sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in servAddr = SocketAddr(AF_INET, PORT, "127.0.0.1");

    char *buffer = new char[MAXLINE];
    memcpy(buffer, "Hello from Client", strlen("Hello from Client"));

    sendto(sockfd, buffer, strlen(buffer), MSG_CONFIRM, (SA *)&servAddr,
           sizeof(servAddr));

    std::cout << "Hello message sent." << std::endl;

    socklen_t len;
    int n =
        recvfrom(sockfd, buffer, MAXLINE, MSG_WAITALL, (SA *)&servAddr, &len);
    buffer[n] = '\0';
    std::cout << "Server: " << buffer << std::endl;
    close(sockfd);
    return 0;
}
