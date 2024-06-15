#pragma once

#include <string>

class Buffer {
private:
    std::string buffer_;

public:
    Buffer() = default;
    ~Buffer() = default;

    void append(const char *str, ssize_t len);
    void getline();
    
    ssize_t size() const { return buffer_.size();}
    const char *c_str() const { return buffer_.c_str(); }
    void clear() { buffer_.clear(); }
};