#pragma once

class InetAddress;

class Socket {
private:
  int fd_;

public:
  Socket();
  Socket(int fd);
  ~Socket();

  void bind(InetAddress *addr);
  void listen();
  int accept(InetAddress *addr);
  void connect(InetAddress *addr);
  void setNonBlocking();

  int fd() const { return fd_; }
};