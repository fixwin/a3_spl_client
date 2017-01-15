CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system -lboost_locale -lboost_thread

all: clean EchoClient
	g++ -o bin/client bin/ConnectionHandler.o bin/Client.o bin/Protocol.o $(LDFLAGS) 
	clear
	./bin/client 10.0.0.21 7777

EchoClient: bin/ConnectionHandler.o bin/Client.o bin/Protocol.o
	
bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/Client.o: src/Client.cpp
	g++ $(CFLAGS) -o bin/Client.o src/Client.cpp

bin/Protocol.o: src/Protocol.cpp
	g++ $(CFLAGS) -o bin/Protocol.o src/Protocol.cpp
	
.PHONY: clean
clean:
	clear
	rm -f bin/*
