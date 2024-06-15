#pragma once

class InetAddress;

class Socket {
private:
    int fd_;
public:
    Socket();
    Socket(int sockfd);
    ~Socket();

    void bind(InetAddress *addr);
    void listen();
    void setNonBlocking();
    int accept(InetAddress *addr);
    int fd() const { return fd_; }
};