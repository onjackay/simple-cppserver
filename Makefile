all: server
server:
	g++ -g client.cpp src/util.cpp src/Socket.cpp src/InetAddress.cpp \
	src/Buffer.cpp -o client && \
	g++ -g server.cpp src/util.cpp src/Epoll.cpp src/InetAddress.cpp \
	src/Socket.cpp src/Channel.cpp src/EventLoop.cpp src/Server.cpp \
	src/Acceptor.cpp src/Connection.cpp src/Buffer.cpp src/ThreadPool.cpp -o server
clean:
	rm server && rm client