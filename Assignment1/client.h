
/*
    File: client.h

    Author: Xiaopei Xu
            Department of CSE
            Texas A&M University
    Date  : 09/19/2016

    Description: Header file of client part to declare the methods and members of client.
*/



#include "utils.h"

class client{
    string username;
    static int sockFd;
   
    
public:
    static clock_t time;
    static bool idle;
    // create a client an connect to server
    client(string name, char* ip, int port);
    
    //Send Join to server
    void join();

    //keep reading from server
    static void * readServer(void*);
    
    //keep writing to server
    static void * writeServer(void*);
    
    //timer (for idle)
    static void * timer(void *);
    
}
;



