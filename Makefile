CC = g++
CFLAGS = -std=c++17
PTHREAD = -lpthread

all: server client

server: server.cpp
		$(CC) $(CFLAGS) -o server server.cpp $(PTHREAD)

client: client.cpp
		$(CC) $(CFLAGS) -o client client.cpp


clean:
	rm -f ./client ./server


