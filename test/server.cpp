#include "Server.h"

#include <memory>

#include "EventLoop.h"

int main() {
    auto loop = std::make_unique<EventLoop>();
    auto server = std::make_unique<Server>(loop.get(), 1145);
    loop->loop();
}