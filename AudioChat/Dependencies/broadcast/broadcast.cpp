#include "broadcast.h"

Broadcast::Broadcast() {}

void Broadcast::addDest(sockaddr_in user) { users.insert(user); }

void Broadcast::sendFrom(sockaddr_in user, int socket, void* buffer,
                         int bufferLen) {
    for (auto addr : users) {
        if (eqsockAddr(addr, user)) continue;
        sendto(socket, buffer, bufferLen, MSG_CONFIRM, (struct sockaddr*)&addr,
               sizeof(addr));
    }
}

bool Broadcast::Cmp::operator()(const sockaddr_in& A,
                                const sockaddr_in& B) const {
    if (A.sin_addr.s_addr == B.sin_addr.s_addr) return A.sin_port < B.sin_port;
    return A.sin_addr.s_addr < B.sin_addr.s_addr;
}

bool Broadcast::eqsockAddr(const sockaddr_in& A, const sockaddr_in& B) {
    return (A.sin_addr.s_addr == B.sin_addr.s_addr) &&
           (A.sin_port == B.sin_port);
}
