all: client proxy

utils: utils.h utils.cpp
	g++ utils.c -o utils.o -pthread

client: client.cpp utils.o
	g++ client.cpp utils.o -o client  -pthread

proxy: proxy.h proxy.cpp utils.o
	g++ proxy.cpp utils.o -o proxy  -pthread

clean:	
	rm -f client proxy *.o