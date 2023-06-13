#include <audio-api.h>
#include <epoll-api.h>
#include <string.h>
#include <unistd.h>
#include <unp.h>

#include <iostream>

#define PORT 8080
#define MAXLINE (256 * sizeof(float))

int main() {
    int sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in servAddr = SocketAddr(AF_INET, PORT, "127.0.0.1");

    // Client address, needed for reply
    socklen_t len;
    sockaddr_in cliAddr;
    socklen_t lenCliAddr;
    mZero(&cliAddr, sizeof(cliAddr));

    void *buffer = malloc(MAXLINE);

    Audio audio;
    audio.initialize();
    audio.openCaptureStream(paFloat32, 8'000, 256, 7300);

    audio.openPlaybackStream(paFloat32, 8'000, 256);

    std::vector<epoll_event> ev(3);
    Epoll epoll;
    epoll.add(audio.getCaptureFD());
    epoll.add(sockfd);

    audio.startCaptureStream();
    audio.startPlaybackStream();

    while (1) {
        int nfds = epoll.wait(ev);
        for (int i = 0; i < nfds; i++) {
            if (ev[i].data.fd == sockfd) {
                int n = recvfrom(sockfd, buffer, MAXLINE, MSG_WAITALL,
                                 (SA *)&cliAddr, &len);
                memcpy(audio.playbackBuffer.rawBuffer, buffer, n);
            } else if (ev[i].data.fd == audio.getCaptureFD()) {
                int n = recvfrom(ev[i].data.fd, buffer, MAXLINE, MSG_WAITALL,
                                 (SA *)&cliAddr, &len);
                n = sendto(sockfd, audio.captureBuffer.rawBuffer, MAXLINE,
                           MSG_CONFIRM, (SA *)&servAddr, sizeof(servAddr));
            }
        }
    }
    close(sockfd);
    return 0;
}
