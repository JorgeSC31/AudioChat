#include <audio-api.h>
#include <config.h>
#include <epoll-api.h>
#include <string.h>
#include <unistd.h>
#include <unp.h>

#include <iostream>

int main() {
    int sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in servAddr = SocketAddr(AF_INET, SERVER_PORT, "127.0.0.1");

    // Client address, needed for reply
    socklen_t len;
    sockaddr_in cliAddr;
    socklen_t lenCliAddr;
    mZero(&cliAddr, sizeof(cliAddr));

    void *buffer = malloc(BUFFER_SIZE);

    Audio audio;
    audio.initialize();
    audio.openCaptureStream(paFloat32, RATE, FRAMES_PER_SECOND,
                            7300);  // PORT FOR CAPTURE STREAM
    audio.openPlaybackStream(paFloat32, RATE, FRAMES_PER_SECOND);

    std::vector<epoll_event> ev(2);
    Epoll epoll;
    epoll.add(audio.getCaptureFD());
    epoll.add(sockfd);

    audio.startCaptureStream();
    audio.startPlaybackStream();

    while (1) {
        int nfds = epoll.wait(ev);
        for (int i = 0, n; i < nfds; i++) {
            if (ev[i].data.fd == sockfd) {
                n = recvfrom(sockfd, buffer, BUFFER_SIZE, MSG_WAITALL,
                             (SA *)&cliAddr, &len);
                memcpy(audio.playbackBuffer.rawBuffer, buffer, n);
            } else if (ev[i].data.fd == audio.getCaptureFD()) {
                n = recvfrom(ev[i].data.fd, buffer, BUFFER_SIZE, MSG_WAITALL,
                             (SA *)&cliAddr, &len);
                n = sendto(sockfd, audio.captureBuffer.rawBuffer, BUFFER_SIZE,
                           MSG_CONFIRM, (SA *)&servAddr, sizeof(servAddr));
            }
        }
    }
    close(sockfd);
    return 0;
}
