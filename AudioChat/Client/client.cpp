#include <audio-api.h>
#include <epoll-api.h>
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

    Epoll epoll;
    epoll.add(sockfd);
    epoll.add(stdin->_fileno);

    std::string msg = "";
    std::vector<epoll_event> ev(2);
    socklen_t len;

    while (msg != "exit") {
        int nfds = epoll.wait(ev);
        for (int i = 0; i < nfds; i++) {
            if (ev[i].data.fd == stdin->_fileno) {
                std::cin >> msg;
                memcpy(buffer, msg.c_str(), sizeof(msg));
                sendto(sockfd, buffer, strlen(buffer), MSG_CONFIRM,
                       (SA *)&servAddr, sizeof(servAddr));
            } else if (ev[i].data.fd == sockfd) {
                int n = recvfrom(sockfd, buffer, MAXLINE, MSG_WAITALL,
                                 (SA *)&servAddr, &len);
                buffer[n] = '\0';
                std::cout << "Server: " << buffer << std::endl;
            }
        }
    }
    close(sockfd);
    return 0;
}
