#pragma once

class InetAddress;

class Socket {
private:
    int sockfd_;
public:
    Socket();
    Socket(int sockfd);
    ~Socket();

    void bind(InetAddress *addr);
    void listen();
    void setnonblocking();
    int accept(InetAddress *addr);
    int fd() const { return sockfd_; }
};