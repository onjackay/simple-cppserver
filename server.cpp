#include "src/Server.h"

#include <memory>

#include "src/EventLoop.h"

int main() {
    // auto loop = std::make_unique<EventLoop>();
    // auto server = std::make_unique<Server>(loop.get(), 1145);
    auto loop = new EventLoop();
    auto server = new Server(loop, 1145);
    loop->loop();
    delete server;
    delete loop;
}