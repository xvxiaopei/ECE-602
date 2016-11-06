all: client server

utils: utils.h utils.c
	g++ utils.c -o utils.o -pthread

client: client.cpp utils.o
	g++ client.cpp utils.o -o Client  -pthread

server: server.h server.cpp utils.o
	g++ server.cpp utils.o -o Server  -pthread

clean:	
	rm -f Client Server *.o