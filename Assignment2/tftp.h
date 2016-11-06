/*
    File: tftp.h

    Author: Xiaopei Xu
            Department of CSE
            Texas A&M University
    Date  : 09/24/2016

    Description: Contains TFTP Servers functions
*/

#include "utils.h"


class tftp_server{
    static int sockFd;  //main socket fd
    static int port_num;
public:
    tftp_server(char* ip, int port);
    
    //Handle service for one RRQ
    static void * serviceManager(void * buff);
    
    
}
;