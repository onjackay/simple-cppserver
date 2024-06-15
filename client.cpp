#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

#include "src/Buffer.h"
#include "src/InetAddress.h"
#include "src/Socket.h"
#include "src/util.h"

#define BUF_SIZE 1024

int main() {
    Socket *sock = new Socket();
    InetAddress *addr = new InetAddress("127.0.0.1", 1145);
    sock->connect(addr);
    // sock->setNonBlocking();

    Buffer *send_buf = new Buffer();
    Buffer *recv_buf = new Buffer();

    while (true) {
        send_buf->getline();
        ssize_t write_bytes =
            write(sock->fd(), send_buf->c_str(), send_buf->size());
        std::cout << "write bytes: " << write_bytes << std::endl;
        if (write_bytes == -1) {
            printf("socket already disconnected, can't write any more!\n");
            break;
        }

        char buf[1024];
        while (true) {
            bzero(&buf, sizeof(buf));
            ssize_t bytes_read = read(sock->fd(), buf, sizeof(buf));
            if (bytes_read > 0) {
                recv_buf->append(buf, bytes_read);
            } else if (bytes_read == -1 && errno == EINTR) {
                printf("continue reading");
                continue;
            } else if (bytes_read == 0) {
                printf("EOF, server fd %d disconnected\n", sock->fd());
                exit(EXIT_SUCCESS);
            }

            if (recv_buf->size() >= send_buf->size()) {
                printf("message from server fd %d: %s\n", sock->fd(),
                       recv_buf->c_str());
                recv_buf->clear();
                break;
            }
        }
    }
    delete sock;
    delete addr;
    delete send_buf;
    delete recv_buf;
    return 0;
}
