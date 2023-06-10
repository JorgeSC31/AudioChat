#pragma once

#include <netinet/in.h>

#include <set>

class Broadcast {
   public:
    Broadcast();
    void addDest(sockaddr_in user);
    void sendFrom(sockaddr_in user, int socket, void* buffer, int bufferLen);

   private:
    bool eqsockAddr(const sockaddr_in&, const sockaddr_in&);
    struct Cmp {
        bool operator()(const sockaddr_in&, const sockaddr_in&) const;
    };

    std::set<sockaddr_in, Cmp> users;
};