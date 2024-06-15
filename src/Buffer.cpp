#include "Buffer.h"

#include <iostream>

void Buffer::append(const char *str, ssize_t len) {
    for (int i = 0; i < len; i++) {
        if (str[i] == '\0') {
            break;
        }
        buffer_.push_back(str[i]);
    }
}

void Buffer::getline() {
    buffer_.clear();
    std::getline(std::cin, buffer_);
}