/*
    File: utils.h

    Author: Xiaopei Xu
            Department of CSE
            Texas A&M University
    Date  : 09/19/2016

    Description: Declare utils function to help write and read sockets, as well as packing and unpacking messages.
*/

#include <cstdio>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <vector>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <map>
#include <iostream>

#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;


#define MAXLINE  1024

enum HType { KEEP0, KEEP1, JOIN, FWD, SEND, NAK, OFFLINE, ACK, ONLINE, IDLE};
enum AType { NoAttr, Reason, Username, Client_Count, Message};


//pack an attribute payload as an SBCP attribute
string packAttr(AType attrType, string attrPayl);

//Add SBCP header to 0 or mor SBCP attributes.
string addHeader(HType headerType,vector<string> payload);

//Get SBCP message type from a header
HType getHType(string SBCPmessage);

//Remove header and get a list of attributes
vector<string> rmHeader(string SBCPmessage);

//Get the attribute type of a SBCP attribute
AType getAType(string SBCPAttribute);

//Get Payload of a SBCP attribute
string getPayl(string SBCPAttribute);


//write
int writeSock(int sockFd, string message);

//read
int readSock(int sockFd,string& message );

//Error exit
void err_sys(const char* x);        


