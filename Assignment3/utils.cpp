/*
    File: utils.cpp

    Author: Xiaopei Xu
            Department of CSE
            Texas A&M University
    Date  : 10/19/2016

    Description: Defination of utils functions.
*/
#include "utils.h"

//Error exit
void err_sys(const char* x)        
{ 
    perror(x); 
    exit(1); 
}

//write
int writeSock(int sockFd, string message)
{
    char buff[MAXLINE];
    int all=0;
    //can't use snprintf cause some char cause terminate earlier
    int i=0;
    for(; i < message.size(); i++){
        buff[i%MAXLINE] = message[i];
        if((i%MAXLINE)==MAXLINE-1){
            all+=write(sockFd,buff,MAXLINE );
            memset(&buff, 0, sizeof(buff));
        }
    }
		
    //cout<<"write"<<endl;
    all+=write(sockFd,buff,message.size()-all);
    return all;		
}

//read
int readSock(int sockFd,string& message )
{
    char buff[MAXLINE];
    message = "";
    int all=0;
    int n=1;
    while(n>0){
        memset(&buff, 0, sizeof(buff));
        n=read(sockFd,buff,MAXLINE);
        for(int i = 0; i < n; i++)
	    	message+=buff[i];
	    all+=n;
	    if(buff[n-1]==EOF) {    //detect EOF,return
	        message = message.substr(0, message.size() - 1);
	        all--;
	        break;
	    }
    }
    
        //Seems we cannot use sprintf to get a string(it will make the string terminate earlier)
    return all;
}

//
int c_web(string host){
    int pyFd;
    struct addrinfo hints, *res, *p;
    int status;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // AF_INET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;
    
    if ((status = getaddrinfo(host.c_str(), "http", &hints, &res)) != 0) {
        char error[100];
        snprintf(error,sizeof(error),"getaddrinfo: %s\n", gai_strerror(status));
        err_sys(error);
    }
    
    //printf("IP addresses for %s:\n\n", m.url->host.c_str());

    for(p = res;p != NULL; p = p->ai_next) {
        if(((pyFd= socket(p->ai_family, p->ai_socktype, p->ai_protocol))>=0 )&&
                (connect(pyFd, p->ai_addr, p->ai_addrlen)>=0))
            break;
    }
    if(p==NULL) pyFd=-1;
    freeaddrinfo(res);
    return pyFd;
    
}


vector<string> split(string str, char delimiter) {
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;
  
  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }
  
  return internal;
}



URL::URL(string url){
    vector<string> sep = split(url,'/');
    
    //schema
    int pos = 0;  
    if(sep[0].compare("http:")==0) pos+=2;
    
    //host and port
    vector<string> hostport = split(sep[pos++],':');
    host = hostport[0];
    if(hostport.size()==2)
        port = hostport[1];
    else port = "80";
        
    //path
    path = (pos==sep.size())?"/":"";
    for(;pos<sep.size();pos++)
     { 
         path+="/";
         path+=sep[pos];
     }
      
 }
 
void URL::print(){
     cout<<"Host: "<<host<<endl;
     cout<<"Port: "<<port<<endl;
     cout<<"Path: "<<path<<endl;
 }
 
string URL::toString(){
    return host+":"+port+path;
}
 
bool URL::equals(URL _url){
    return (host.compare(_url.host)==0 && port.compare(_url.port)==0 && path.compare(_url.path)==0);
}
 
 
Message::Message(string m){
    vector<string> sep = split(m,' ');
    method = sep[0];
    url = new URL(sep[1]);
    version = sep[2];
}

void Message::print(){
    cout<<"Method: "<<method<<endl;
    url->print();
    cout<<"Version: "<<version<<endl;
}

string Message::toString(bool condition,string access_date){
    return method + " " + url->path + " " + version + "\r\n"
        +"Host: " + url->host + "\r\n"
        +"User-Agent: HTTPTool/1.0\r\n"
        +((condition)?"If-Modified-Since: "+access_date+"\r\n":"")
        +"\r\n";
}

Document::Document(URL* _url, string message){
    url = _url;
    //Get date, if not exist, use current GMT time
    size_t found = message.find("Date: ");
    if (found==string::npos) {
        time_t now = time(NULL);
        date = *gmtime(&now);
    }
    else date = timeStamp(message.substr(found+6,29));
    //Get Expires time, if not exist, use current GMT time + 24 hours
    found = message.find("Expires: ");
    if (found==string::npos) {
        time_t now = time(NULL);
        expire_time = *gmtime(&now);
        expire_time.tm_mday++;
    }
    else expire_time = timeStamp(message.substr(found+9,29));
    
    //Get Last Modify time, if not exist, use current GMT time -1 month
    found = message.find("Last-Modified: ");
    if (found==string::npos) {
        time_t now = time(NULL);
        modify_time = *gmtime(&now);
        modify_time.tm_mon--;
        if(modify_time.tm_mon<0){
            modify_time.tm_mon=11;
            modify_time.tm_year--;
        }
    }
    else modify_time = timeStamp(message.substr(found+15,29));
    
    //find body
    found = message.find("<");
    if (found==string::npos) {body="";return;}
    else body = message.substr(found);
    
}



bool createFile(string content,string name){
    const char* path = name.c_str();
    ofstream fout(path);
    if ( fout ) { 
        fout << content << endl; 
        fout.close();  
        return true;
    }
    return false;
}

string timeName(){
    time_t currtime = time(NULL);  
    tm* p = localtime(&currtime);  
    char filename[100] = {0};  
  
    sprintf(filename,"%d%02d%02d%02d%02d%02d",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);  
    return (string)filename;   
    
}

struct tm timeStamp(string timeheader){
    static const char format[] = "%a, %d %b %Y %H:%M:%S %Z"; // rfc 1123
    struct tm tm;
    bzero(&tm, sizeof(tm));
    strptime(timeheader.c_str(), format, &tm);
    return tm;
}

string timeStr(struct tm  tm){
    const char* fmt = "%a, %d %b %Y %H:%M:%S GMT";
    char buff[30];

    strftime(buff, sizeof(buff), fmt, &tm);
    return (string)buff;
    
}

void Document::print(){
    cout<<url->toString()<<endl;
    cout<<"Access Date: "<<timeStr(date)<<"     Expires: "<<timeStr(expire_time)<<"      Last_Modified: "<<timeStr(modify_time)<<endl;
    cout<<"Fresh: "<<fresh()<<endl;
}

 bool Document::fresh(struct tm* cur )  //determine if this document is still fresh, default will be current time
 {
    time_t now = cur == NULL? time(NULL):mktime(cur);
    time_t ex = mktime(&expire_time);
    time_t md = mktime(&modify_time);
    
    return difftime(now,md)>=2592000 && difftime(now,ex)<0;   //now is earlier than expire time while last modified time is one month ago
     
 }