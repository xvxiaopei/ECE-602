This is the Network Programming Assignment #3 project of ECEN 602, FALL 2016(with bonus part). 

Team 8:  Xiaopei Xu，  Yanxiang Yang

In this project we implement a simple HTTP proxy server and HTTP command line client, which work for HTTP/1.0.
Proxy server has a LRU cache of 10 documents entries.(which can improve by only modify a single integer varible in proxy.cpp).

To compile this project, simply type in 'make' to call the Makefile.Use 'make clean' to clean compiled file.

The server can be put online by calling the command : ./proxy <ip to bind> <port to bind> 
The client can be called by the command             : ./client <proxy address> <proxy port> <URL to retrieve>.

Then the server will print some log in stdout, and client will get the document and save it in current directory 
of host name(e.g. client to get "www.google.com" will get a document named "www.google.com" in current directory).

Implementation:

1. Use thread to handle multiple clients requests, when a client's request come to proxy, server will create a thread to handle it;

2. Use strptime, time_t, timediff and some other functions and types to store and compare timestamp(so this implementation can only work in Linux environment);

3. Use a linked list to implement LRU. In each node, there is a Document instance, which contains the URL, access date, expired date,
    last_modify date and document body of this file; every time we add a new document in cache, we add it at the head of the linked list,
    and every time after we "find" a document in cache, we put founded document at the first of the linked list;
    
4. When the server get a response from web server, it will parse the HTTP response to get the times and body it need and create a 
    Document instance and store it in cache;

5. When parsing the HTTP response, server can get access date. And if there is no expired date, we use "access date + 24 hours" as 
    expired date. If there is no last modify date, we use "access date - 1 month " as last modified date.
    In this way, when determint if a document is expired, we follow the logic:
        A document is fresh if last modify time is one month earlier, and current time is ealier than expired time.
    Which will be consistent with the requirements in instruction of NPA3.

6. When the server get a request from the client, it will firstly parse the message and get url(host, port and path). Then it will
    follow the steps:
        1. check if there is a document with the same URL in cache;
        2. if there is such a document and it is not expired, get the document, go to step 6; else step 3
        3. use the host name to find the address of the host(getaddrinfo), and connect to the host;
        4. if there is a expired document in cache, send a conditional GET to request host;
                if the host response with "304 Not Modified", then get the expire document, go to step 6; 
                else the host will response normally, parse the message and get document, add into cache, go to step 6; 
        5. if there is no such a document in cache, simply send GET to host server and parse the response, add into cache, go to step 6;
        6. send back the document to client
        
7. Client only send Request-URL with GET, and after get response from proxy server, it will save the file in current directory.
    
