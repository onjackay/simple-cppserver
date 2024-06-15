#pragma once
#include <arpa/inet.h>

class InetAddress {
   private:
   public:
    struct sockaddr_in addr_;
    socklen_t addr_len_;

    InetAddress();
    InetAddress(const char *ip, uint16_t port);
    ~InetAddress();

    struct sockaddr *addr() {
        return reinterpret_cast<struct sockaddr *>(&addr_);
    }
    socklen_t *addrLen() { return &addr_len_; }

    const char *ip() const { return inet_ntoa(addr_.sin_addr); }
    uint16_t port() const { return ntohs(addr_.sin_port); }
};