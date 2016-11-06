/*
    File: proxy.h

    Author: Xiaopei Xu
            Department of CSE
            Texas A&M University
    Date  : 10/19/2016

    Description: Header file of proxy part to declare the methods and members of proxy.
*/

#include "utils.h"

//Node of linkedlist
class ListNode{
public:
    ListNode * next;
    Document * doc;
    ListNode();
    ListNode(URL* url,string message);
};

class cache{
public:
    int limit;
    int curNum;
    ListNode * head;
    cache(int _limit);

    Document* find(URL _url);
    void add(URL* _url,string message);//if full,remove
    void print();
    bool deleteHead();   //delete head (when it is expired)
             
     
};

class Proxy{
public:
    static int sockFd;  
    Proxy(char* ip, int port);
    static cache LRU;
    //Handle service for one client
    static void * serviceManager(void * clientFd);
    
    
};

