#include "epoll-api.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Epoll::add(int socket, int events) {
    sharedEvent.events = events;
    sharedEvent.data.fd = socket;
    Epoll_ctl(epollFD, EPOLL_CTL_ADD, socket, &sharedEvent);
}

void Epoll::del(int socket) { Epoll_ctl(epollFD, EPOLL_CTL_DEL, socket, NULL); }

int Epoll::wait(std::vector<epoll_event> &events, int timeout = -1) {
    int n;
    if ((n = epoll_wait(epollFD, events.data(), events.size(), timeout)) < 0) {
        printf("ERROR Epoll::wait : %s\n", strerror(errno));
        exit(1);
    }
    return n;
}

void Epoll_ctl(int epollFD, int option, int fd, epoll_event *event) {
    if (epoll_ctl(epollFD, option, fd, event) < 0) {
        printf("ERROR Epoll_ctl : %s\n", strerror(errno));
        exit(1);
    }
}