/*
    File: server.cpp

    Author: Xiaopei Xu
            Department of CSE
            Texas A&M University
    Date  : 09/19/2016

    Description: Definations of server methods and main function for execution.
*/

#include "server.h"

int main(int argc, char** argv)
{
    printf("Open server\n");

    if(argc != 4)
        err_sys("Number of arguments error\n");
        
    server sv(argv[1],atoi(argv[2]),atoi(argv[3]));
}

int server::sockFd;    
map<string,int> server::users;
int server::userLimit;
int server::clientCount;

server::server(char* ip, int port,int limit)
{
    userLimit = limit;
    //Create socket
    if((sockFd = socket(AF_INET,SOCK_STREAM,0))>=0 )    
    { //ipv6
         //Initialize addr
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(ip);
        addr.sin_port = htons(port);
        
         //Bind
        if (bind(sockFd, (struct sockaddr *) &addr, sizeof(addr) ) < 0)
		    err_sys("bind error\n");
    }
    else if((sockFd = socket(AF_INET6,SOCK_STREAM,0))>=0)
    {
        //ipv6
        struct sockaddr_in6 addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin6_family = AF_INET6;
        inet_pton(AF_INET6, ip, &addr.sin6_addr);
        addr.sin6_port = htons(port);
        
         //Bind
        if (bind(sockFd, (struct sockaddr *) &addr, sizeof(addr) ) < 0)
		err_sys("bind error\n");
    }else
        err_sys("IP error\n");
    
    //Listen
	if ( listen(sockFd, 10) < 0)
		err_sys("listen error\n");
	printf("Server is online\n");
	
	//Online
	char buff[MAXLINE];
	while(1){
	    int clientFd;
	    if((clientFd =  accept(sockFd, (struct sockaddr *) NULL, NULL) ) < 0)
			err_sys("accept error\n");
		
		pthread_t st;
		pthread_create(&st, NULL, server::serviceManager, (void*)(intptr_t)clientFd);
	}
}



//Handle service for one client
void * server::serviceManager(void* clientFd)
{
    string message;
    int clFd = (intptr_t)clientFd;
    string name = "";
    bool join = false;
    int n=0;
    while((n=readSock(clFd,message))>0){
		switch(getHType(message)){
		    case JOIN: join=opJoin((name = getPayl(rmHeader(message)[0])),clFd);break;
		    case SEND: opSend(name,getPayl(rmHeader(message)[0]));break;
		    case IDLE: onfiline(name,IDLE);break;
		}
    }
    if(n==0 && join){//if a client is offline
        for(map<string,int>::iterator it=users.begin();it!=users.end();)
        {  
            if (it->first==name)  
                users.erase(it++); 
            else it++;
        } 
        clientCount--;
        printf("%s leaves chat.\n",name.c_str());
        onfiline(name,OFFLINE);
    }
    if(n<0) {//error reading
        char error[100];
        snprintf(error,sizeof(error),"read error of client fd: %d\n",clFd);
        err_sys(error);
    }
}

//deal with join from a client
bool server::opJoin(string name,int clientFd)
{
    if(clientCount>=userLimit)//can't accept more user
    {
        nak(clientFd,"Can't accept more users.");
        return false;
    }
    else if ( users.find(name.c_str()) == users.end() ) {
        //not found, send ACK
        clientCount++;
        ack(clientFd);
        onfiline(name,ONLINE);
        users[name] = clientFd;
        printf("%s joins chat.\n",name.c_str());
        return true;
    } 
    
    else{// found same username, send NAK
        nak(clientFd,"User name is already in use.");
        return false;
    }
        
}

//deal with send from a client
void server::opSend(string name,string message)
{
    if (users.find(name.c_str()) == users.end())
        return;    //Can't recognize the client
    //printf("Get a message from %s : %s\n",name.c_str(),message.c_str());
    fwd(name,message);
}

//server sends to client to confirm the JOIN requesr
void server::ack(int clientFd)
{
    vector<string> attrs;
    // user count
    string message_count="";
    message_count+=(char)(clientCount/256);
    message_count+=(char)(clientCount%256);
    attrs.push_back(packAttr(Client_Count,message_count));
    
    //username(s)
    map<string,int>::iterator it;
    for(it=users.begin();it!=users.end();++it)
        attrs.push_back(packAttr(Username,it->first));

    writeSock(clientFd,addHeader(ACK,attrs));
    //printf("send ACK!!\n");
}

//reject a join with a reason
void server::nak(int clientFd,string reason)
{
    vector<string> attrs;
    attrs.push_back(packAttr(Reason,reason));
    writeSock(clientFd,addHeader(NAK,attrs));
    //printf("send NAK!!\n");
}

//indicate arrival/departure of a user
void server::onfiline(string username,HType onf)
{
    vector<string> attrs;
    attrs.push_back(packAttr(Username,username));
    
    map<string,int>::iterator it;
    //send online/offline message to every user
    for(it=users.begin();it!=users.end();++it){
        writeSock(it->second,addHeader(onf,attrs));
    }
}

//fwd a chat text
void server::fwd(string name,string message)
{
    vector<string> attrs;
    attrs.push_back(packAttr(Username,name));
    attrs.push_back(packAttr(Message,message));
    
    map<string,int>::iterator it;
    //send message to every user except sender
    for(it=users.begin();it!=users.end();++it){
        if(it->first == name) continue;
        writeSock(it->second,addHeader(FWD,attrs));
    }
    
}
