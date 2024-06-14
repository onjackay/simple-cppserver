#include "InetAddress.h"
#include "util.h"
#include <cstring>

InetAddress::InetAddress() {
    memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_len_ = sizeof(addr_);
}

InetAddress::InetAddress(const char *ip, uint16_t port) {
    memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    addr_.sin_addr.s_addr = inet_addr(ip);
    addr_len_ = sizeof(addr_);
}

InetAddress::~InetAddress() {}
