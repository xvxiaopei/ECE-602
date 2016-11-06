/*
    File: proxy.cpp

    Author: Xiaopei Xu
            Department of CSE
            Texas A&M University
    Date  : 10/19/2016

    Description: Definations of proxy methods and main function for execution.
*/

#include "proxy.h"

int main(int argc, char** argv)
{
    printf("Open proxy server\n");

    if(argc != 3)
        err_sys("Number of arguments error\n");
    
    Proxy py(argv[1],atoi(argv[2]));
}

int Proxy::sockFd;
cache Proxy::LRU(10);   //set the size of LRU cache


Proxy::Proxy(char* ip, int port)
{
    //Create socket
    if((sockFd = socket(AF_INET,SOCK_STREAM,0))<0 )   
        err_sys("Socket creation error.");
    
    //Initialize addr
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
        
    //Bind
    if (bind(sockFd, (struct sockaddr *) &addr, sizeof(addr) ) < 0)
		err_sys("bind error\n");
	
	//lisen
	if ( listen(sockFd, 10) < 0)
		err_sys("listen error\n");
	printf("Server is online\n\n");
	
	//Online
	char buff[MAXLINE];
	while(1){
	    int clientFd;
	    if((clientFd =  accept(sockFd, (struct sockaddr *) NULL, NULL) ) < 0)
			err_sys("accept error\n");
		
		pthread_t st;
		pthread_create(&st, NULL, Proxy::serviceManager, (void*)(intptr_t)clientFd);
	}
    
}

//Handle service for one client
void * Proxy::serviceManager(void* clientFd)
{
    string message;
    int clFd = (intptr_t)clientFd;
    int pyFd;  //socket : connect to web server
    int n=0;
    if((n=readSock(clFd,message))<0){
        char error[100];
        snprintf(error,sizeof(error),"read error of client fd: %d\n",clFd);
        err_sys(error);
    }
  
   	Message m(message);
	cout<<"Get a request: "<<message<<endl;
	
	//Then check cache to get the file or request web server
	Document* d = LRU.find(*(m.url));
	string response;
	if(d!=NULL && d->fresh()){
	    cout<<"Request URL: "<<m.url->toString()<<" is in cache and valid."<<endl;
	}
	else {
	    // Connect to web server 
        pyFd = c_web(m.url->host);
        if(pyFd==-1) err_sys("Cannot connect to web server.");
    	cout<<"Connect to host: "<<m.url->host<<endl;
        
        //Bonus Part:check if this expired file can be used
        if(d!=NULL)   //has expired document in cache, check if it can still be used
        {
            cout<<"Request URL: "<<m.url->toString()<<" is in cache but expired."<<endl;
            writeSock(pyFd,m.toString(true,timeStr(d->date)));
            readSock(pyFd,response);
            cout<<"Get response from host: "<<m.url->host<<endl;
            if(response.find("304 Not Modified")==string::npos) {//modified
                LRU.deleteHead();   //deletet the expired document
	            cout<<"Document can't be used and need to be refreshed."<<endl;
                LRU.add(m.url,response);
                d = LRU.head->doc;
            }
            else cout<<"Document can be used since it wasn't modified since last access time."<<endl;
        }
    	else {// not in cache
    	    cout<<"Request URL: "<<m.url->toString()<<" is not in cache."<<endl;
    	    writeSock(pyFd,m.toString());
        	readSock(pyFd,response);
        	cout<<"Get response from host: "<<m.url->host<<endl;
            LRU.add(m.url,response);
            d = LRU.head->doc;
    	}
	}
    
    string newM = d->body;
    //string newM = response;
    newM+=EOF;
    writeSock(clFd,newM);
    cout<<"Send back document to client."<<endl;
    cout<<"Now the cache is: "<<endl;
    LRU.print();
    cout<<endl;
}

ListNode::ListNode(URL * url,string message){
    doc = new Document(url,message);
}
ListNode::ListNode(){}

cache::cache(int _limit){
    curNum = 0;
    limit = _limit;
    head = NULL;
}

Document* cache::find(URL _url){
    ListNode* fake = new ListNode();
    fake->next = head;
    ListNode * p = fake;
    
    while(p->next!=NULL){
        if(p->next->doc->url->equals(_url)){
            //put First
            if(p!=fake)  
            {
                ListNode *q = p->next;
                p->next = p->next->next;
                q->next = head;
                head = q;
            }
            delete fake;
            return head->doc;
        }
        p=p->next;
    }
   return NULL;
    
}

bool cache::deleteHead(){
    if(head==NULL) return false;
    curNum--;
    ListNode *p=head;
    head=head->next;
    delete p;
    return true;
            
}

void cache::add(URL* _url,string message){
    ListNode * p = new ListNode(_url,message);
    p->next =head;
    head = p;
    if(++curNum>limit){
        for(int i=0;i<limit-1;i++){
            p=p->next;
        }
        ListNode * fake = p->next;
        p->next = NULL;
        delete fake;
    }
    // cout<<"After add, head->next "<<((head->next==NULL)?"=NULL":"!=NULL")<<endl;
    // if(p->next!=NULL)cout<<"After add, p->next->next "<<((p->next->next==NULL)?"=NULL":"!=NULL")<<endl;
}

void cache::print(){
    ListNode *p=head;
    int i=0;
    while(p!=NULL){
        cout<<i<<" : "; 
        p->doc->print();
        p=p->next;
        i++;
    }
    
}

