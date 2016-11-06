/*
    File: server.h

    Author: Xiaopei Xu
            Department of CSE
            Texas A&M University
    Date  : 09/19/2016

    Description: Header file of server part to declare the methods and members of server.
*/

#include "utils.h"

class server{
    static int sockFd;    
    static map<string,int> users;
    static int userLimit;
    static int clientCount;
public:
    server(char* ip, int port,int limit);
    
    //Handle service for one client
    static void * serviceManager(void * clientFd);
    
    //deal with join from a client
    static bool opJoin(string name,int clientFd);
    
    //deal with send from a client
    static void opSend(string name,string message);
    
    //server sends to client to confirm the JOIN requesr
    static void ack(int clientFd);
    
    //reject a request
    static void nak(int clientFd, string reason);
    
    //fwd a chat text
    static void fwd(string name, string message);
    
    //indicate arrival/departure/idle of a user
    static void onfiline(string username,HType onf);
}
;