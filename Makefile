all: server
server:
	g++ -g client.cpp src/util.cpp -o client && \
	g++ -g server.cpp src/util.cpp src/Epoll.cpp src/InetAddress.cpp src/Socket.cpp src/Channel.cpp src/EventLoop.cpp src/Server.cpp src/Acceptor.cpp -o server
clean:
	rm server && rm client