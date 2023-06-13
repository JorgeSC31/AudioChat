#include <audio-api.h>
#include <epoll-api.h>
#include <string.h>
#include <unistd.h>
#include <unp.h>

#include <iostream>

#define PORT 8080
#define MAXLINE (256 * sizeof(float))

void test() {
    int sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in servAddr = SocketAddr(AF_INET, PORT + 1, "127.0.0.1");
    Bind(sockfd, servAddr);

    char msg[] = "Hola mundo cruel";
    char *buffer = (char *)malloc(MAXLINE);

    socklen_t len;
    int n;
    n = sendto(sockfd, msg, strlen(msg), MSG_CONFIRM, (SA *)&servAddr,
               sizeof(servAddr));
    printf("%d %s\n", n, strerror(errno));
    n = recvfrom(sockfd, buffer, MAXLINE, MSG_WAITALL, (SA *)&servAddr, &len);
    printf("%d %s\n", n, strerror(errno));

    buffer[n] = '\0';
    printf("Mensaje: %s\n", buffer);

    // exit(0);
}

int main() {
    test();
    int sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in servAddr = SocketAddr(AF_INET, PORT, "127.0.0.1");

    char *buffer = new char[MAXLINE];
    memcpy(buffer, "Hello from Client", strlen("Hello from Client"));

    Epoll epoll;
    epoll.add(sockfd);
    epoll.add(stdin->_fileno);

    Audio audio;
    audio.initialize();
    audio.openCaptureStream(paFloat32, 8'000, 256, 7300);
    epoll.add(audio.getCaptureFD());

    std::string msg = "";
    std::vector<epoll_event> ev(3);
    socklen_t len;

    audio.startCaptureStream();

    while (msg != "exit") {
        int nfds = epoll.wait(ev);
        for (int i = 0; i < nfds; i++) {
            // if (ev[i].data.fd == stdin->_fileno) {
            //     std::cin >> msg;
            //     memcpy(buffer, msg.c_str(), sizeof(msg));
            //     sendto(sockfd, buffer, strlen(buffer), MSG_CONFIRM,
            //            (SA *)&servAddr, sizeof(servAddr));
            // } else
            if (ev[i].data.fd == sockfd) {
                int n = recvfrom(sockfd, buffer, MAXLINE, MSG_WAITALL,
                                 (SA *)&servAddr, &len);
                memcpy(audio.playbackBuffer.rawBuffer, buffer, n);
                // buffer[n] = '\0';
                std::cout << "Server mensaje recibido: " << std::endl;
            } else if (ev[i].data.fd == audio.getCaptureFD()) {
                int n = recvfrom(ev[i].data.fd, buffer, MAXLINE, MSG_WAITALL,
                                 (SA *)&servAddr, &len);
                std::cout << "Capturando audio\n";
                sendto(sockfd, audio.captureBuffer.rawBuffer, MAXLINE,
                       MSG_CONFIRM, (SA *)&servAddr, sizeof(servAddr));
            }
        }
    }
    close(sockfd);
    return 0;
}
