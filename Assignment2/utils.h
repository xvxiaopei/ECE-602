/*
    File: utils.h

    Author: Xiaopei Xu
            Department of CSE
            Texas A&M University
    Date  : 09/19/2016

    Description:  Include libraries and util functions
*/


#include <cstdio>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <vector>
#include <time.h>
#include <pthread.h>
#include <map>
#include <iostream>
#include <fstream>

#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

#define DATA_FIELD 512
enum OPcode {KEEP, RRQ, WRQ, DATA, ACK, ERROR};
enum ErrorCode {NOT_DEFINE,FILE_NOT_FOUND,ACCESS_DENY,DISK_FULL, ILLEGAL_OP, UNKNOWN_TID,FILE_EXIST};

struct info{
    OPcode type;
    string file_name;
    string mode;
    struct sockaddr_in client_addr, server_addr;
}
;


//Error exit
void err_sys(const char* x)       
{ 
    perror(x); 
    exit(1); 
}

//Get opcode
OPcode getOp(char * buff)
{
    return (OPcode)(int)(buff[1]);
    
}