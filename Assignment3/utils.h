/*
    File: utils.h

    Author: Xiaopei Xu
            Department of CSE
            Texas A&M University
    Date  : 10/19/2016

    Description: Declare utils functions.
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
#include <fstream>
#include <sstream>
#include <vector>

#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>


using namespace std;

#define MAXLINE  1024
//split a string by delimiter into a vector of strings
vector<string> split(string str, char delimiter);

//write
int writeSock(int sockFd, string message);

//read
int readSock(int sockFd,string& message );

//Error exit
void err_sys(const char* x);        

//Find the ip address for host, connect web server, return fd
int c_web(string host);

class URL{
public:
  string host;
  string port;
  string path;
  URL(string url);
  void print();
  bool equals(URL _url);
  string toString();
};

class Message{
public:
    URL* url;
    string method;
    string version;
    Message(string m);
    void print();
    string toString(bool condition=false,string access_date="");   //return a string to send to web server(conditional get)
    
    
};

class Document{
public:
    URL* url;    //from where this document has been got

    string body;
    struct tm date;
    struct tm expire_time;
    struct tm modify_time;
    Document(URL* _url, string message);
    void print();
    
    bool fresh(struct tm* cur =NULL);  //determine if this document is still fresh, default will be current time
}
;

//create and save file
bool createFile(string content,string name);

//get current time as file name
string timeName();

// //parse HTML response into initial line, headerlines(vector) and body
// vector<string> parseHTML(string message,string& string& body);

//get time from header
struct tm timeStamp(string timeheader);

//convert time to string
string timeStr(struct tm  tm);

