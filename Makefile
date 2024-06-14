all: server
server:
	g++ -g src/util.cpp client.cpp -o client && \
	g++ -g src/util.cpp server.cpp src/Epoll.cpp src/InetAddress.cpp src/Socket.cpp src/Channel.cpp -o server
clean:
	rm server && rm client