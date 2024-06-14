all: server
server:
	g++ src/util.cpp src/client.cpp -o client && \
	g++ src/util.cpp src/server.cpp src/Epoll.cpp src/InetAddress.cpp src/Socket.cpp src/Channel.cpp -o server
clean:
	rm server && rm client