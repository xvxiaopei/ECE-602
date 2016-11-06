/*
    File: client.cpp

    Author: Xiaopei Xu
            Department of CSE
            Texas A&M University
    Date  : 10/19/2016

    Description: Definations of client methods and main function for execution.
*/



#include "utils.h"

int main(int argc, char** argv){
    
    printf("Open client\n");
    
    //Arguments check
    if(argc != 4)
        err_sys("Number of arguments error\n");
        
    string message = "GET " + (string)(argv[3]) + " HTTP/1.0";
    cout<<message<<endl;
    
    Message m(message);
    //Create socket
    int sockFd;
    if((sockFd = socket(AF_INET,SOCK_STREAM,0))<0 )    
        err_sys("Socket creation error.");
        
    //Initialize addr
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, argv[1], &addr.sin_addr) <= 0) 
		err_sys("inet_pton error for ");
    addr.sin_port = htons(atoi(argv[2]));
        
    //Connect
    if (connect(sockFd, (struct sockaddr *) &addr, sizeof(addr)) < 0) 
	    err_sys("Connect error");
	cout<<"Connected."<<endl;
	
	message +=EOF;
	
	//Request
	writeSock(sockFd,message);
	cout<<"Send request."<<endl;
	
	//Get response
	string response;
	readSock(sockFd,response);
	cout<<"Get response."<<endl;

    //Save file
	string filename = m.url->host;
	createFile(response,filename);
	cout<<"Save document as '"<<filename<<"'"<<endl;
    
    
}