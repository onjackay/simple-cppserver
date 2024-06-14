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

    struct sockaddr *addr() { return (struct sockaddr *)&addr_; }
    socklen_t *addr_len() { return &addr_len_; }
};