all: server client
server:
	g++ -g server.cpp src/util.cpp src/Epoll.cpp src/InetAddress.cpp \
	src/Socket.cpp src/Channel.cpp src/EventLoop.cpp src/Server.cpp \
	src/Acceptor.cpp src/Connection.cpp src/Buffer.cpp src/ThreadPool.cpp -o server
client:
	g++ -g client.cpp src/util.cpp src/Socket.cpp src/InetAddress.cpp \
	src/Buffer.cpp -o client
test:
	g++ -g test.cpp src/util.cpp src/Epoll.cpp src/InetAddress.cpp \
	src/Socket.cpp src/Channel.cpp src/EventLoop.cpp src/Server.cpp \
	src/Acceptor.cpp src/Connection.cpp src/Buffer.cpp src/ThreadPool.cpp -o test
clean:
	rm server client test