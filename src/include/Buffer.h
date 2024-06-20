#pragma once

#include <string>

class Buffer {
private:
  std::string buf_;

public:
  Buffer() = default;
  ~Buffer() = default;

  void append(const char *str, ssize_t len);
  void getline();

  ssize_t size() const { return buf_.size(); }
  const char *c_str() const { return buf_.c_str(); }
  void clear() { buf_.clear(); }
};