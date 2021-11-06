CXX = g++
CXXFLAGS = -g -Wall -std=c++17

all:	serv cli

serv:	udp_ping_server.cpp
	g++ udp_ping_server.cpp -o server

cli:	udp_ping_client.cpp
	g++ udp_ping_client.cpp -o client

.phony:		clean, runserver, runclient
clean:
		rm -f *- *.o server client

runserver:
		./server

runclient:
		./client
