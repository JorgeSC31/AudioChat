#pragma once

#include <sys/epoll.h>

#include <vector>

class Epoll {
   public:
    Epoll();

    void add(int socket, int events = EPOLLIN);
    void del(int socket);

    int wait(std::vector<epoll_event> &events, int timeout = -1);

   private:
    int epollFD;
    epoll_event sharedEvent;
};

void Epoll_ctl(int epollFD, int option, int fd, epoll_event *event);