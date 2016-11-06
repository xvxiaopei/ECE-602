/*
    File: client.cpp

    Author: Xiaopei Xu
            Department of CSE
            Texas A&M University
    Date  : 09/19/2016

    Description: Definations of client methods and main function for execution.
*/



#include "client.h"

int main(int argc, char** argv){
    printf("Open client %s\n",argv[1]);
    
    //Arguments check
    if(argc != 4)
        err_sys("Number of arguments error\n");
        
    client cl(argv[1],argv[2],atoi(argv[3]));
    cl.join();
    // cl.join();
    while(1){}
}

int client::sockFd;
clock_t client::time;
bool client::idle = false;
client::client(string name, char* ip, int port)
{
    username = name;
    
    
     //Create socket
    if((sockFd = socket(AF_INET,SOCK_STREAM,0))>=0 )    
    { //ipv6
         //Initialize addr
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(ip);
        addr.sin_port = htons(port);
        
        //Connect
        if (connect(sockFd, (struct sockaddr *) &addr, sizeof(addr)) < 0) 
		    err_sys("connect error");
        
    }
    else if((sockFd = socket(AF_INET6,SOCK_STREAM,0))>=0)
    {
        //ipv6
        struct sockaddr_in6 addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin6_family = AF_INET6;
        inet_pton(AF_INET6, ip, &addr.sin6_addr);
        addr.sin6_port = htons(port);
        
        
        //Connect
        if (connect(sockFd, (struct sockaddr *) &addr, sizeof(addr)) < 0) 
		    err_sys("connect error");
    }else
        err_sys("IP error\n");
}

//Send Join to server
void client::join()
{
    string attr = packAttr(Username,username);
    vector<string> attrs;
    attrs.push_back(attr);
    string message = addHeader(JOIN,attrs);
    //send JOIN to server
    writeSock(sockFd,message);
    printf("Send Join...\n");
    
    sleep(1);
    
    //Get ACK or NAK from server
    string rc;
    readSock(sockFd,rc);
    //printf("%d\n")
    if(getHType(rc)==ACK){
        printf("Get accepted!\n");
        vector<string> rc_attrs = rmHeader(rc);
        if(getAType(rc_attrs[0])!=Client_Count)
            err_sys("Didn't get client count!\n");
        string cc = getPayl(rc_attrs[0]);
        int numOfUsers=cc[0]*256+cc[1];
        printf("Now the server has %d user(s)\n",numOfUsers);
        
        if(numOfUsers>1) 
        {
            //then print other users
            printf("Other users are: \n");
            for(int i=1; i<numOfUsers;i++){
                if(getAType(rc_attrs[i])!=Username)
                    err_sys("Didn't get user name!\n");
                printf("  %s\n",getPayl(rc_attrs[i]).c_str());
            }
        }
        // create threads for read from and write to server
        pthread_t rt;
	    pthread_t wt;
	    pthread_t tm;
        pthread_create(&rt, NULL, &client::readServer, NULL);
        pthread_create(&wt, NULL, &client::writeServer, NULL);
        pthread_create(&tm, NULL, &client::timer, NULL);
        
    }
    else if (getHType(rc)==NAK){
        printf("Get rejected!\n");
        vector<string> rc_attrs = rmHeader(rc);
        if(getAType(rc_attrs[0])!=Reason)
            err_sys("Didn't get the reason!\n");
        printf("Reason is : %s\n",getPayl(rc_attrs[0]).c_str());
        exit(0);
    }
    else 
        err_sys("Didn't get reply!\n");
}

//keep reading from server
void* client::readServer(void*)
{
    string message;
    int n=0;
    while((n=readSock(sockFd,message))>0){
		
		switch(getHType(message)){
		    case ONLINE:printf("User %s is online.\n",getPayl(rmHeader(message)[0]).c_str());break;
		    case OFFLINE:printf("User %s is offline.\n",getPayl(rmHeader(message)[0]).c_str());break;
		    case FWD:printf("%s: %s\n",getPayl(rmHeader(message)[0]).c_str(),getPayl(rmHeader(message)[1]).c_str());break;
		    case IDLE:printf("User %s is idle.\n",getPayl(rmHeader(message)[0]).c_str());break;
		}
    }
    if(n==0) {
        close(sockFd);
        err_sys("Server is down\n");
    }
    if(n<0) {
        err_sys("Read error from server\n");
    }
}
    
//keep writing to server
void* client::writeServer(void*)
{
    string message;
    while(1){
        getline (cin, message);
        printf("Me: %s\n",message.c_str());
        vector<string> attrs;
        attrs.push_back(packAttr(Message,message));
        writeSock(sockFd,addHeader(SEND,attrs));
        time = clock();
        idle = false;
    }
}
    
    
//timer (for idle)
void * client::timer(void *)
{
    time = clock();
    while(1){
        if(((double)(clock()-time)/CLOCKS_PER_SEC <10)||idle)
        {
            sleep(1);continue;
        }
        vector<string> attrs;
        writeSock(sockFd,addHeader(IDLE,attrs));
        idle=true;
    }
    
}