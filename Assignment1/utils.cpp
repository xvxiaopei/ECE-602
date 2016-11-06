/*
    File: utils.cpp

    Author: Xiaopei Xu
            Department of CSE
            Texas A&M University
    Date  : 09/19/2016

    Description: Defination of utils functions.
*/
#include "utils.h"

//Pack an attribute payload as an SBCP attribute
string packAttr(AType attrType, string attrPayl)
{
    char attrField[4];
    
    attrField[0] = 0;
    attrField[1] = attrType;
    attrField[2] = attrPayl.size()/256;
    attrField[3] = attrPayl.size()%256;
    
    string res = "";
    for(int i=0; i<4; i++)
        res+=attrField[i];

    return res + attrPayl;
}

//Add SBCP header to 0 or mor SBCP attributes.
string addHeader(HType headerType,vector<string> payload)
{
    char header[4];
    
    header[0] = 1;
    header[1] = 128;     //thus the first 9 bit will indicate 3
    header[1] += headerType;
    header[2] = payload.size()/256;
    header[3] = payload.size()%256;
    
    string res = "";
    for(int i=0; i<4; i++)
        res += header[i];
    
    for(int i=0;i<payload.size();i++)
        res += payload[i];

    return res;
}

//Get SBCP message type from a header
HType getHType(string SBCPmessage)
{
    return (HType)(int)(SBCPmessage[1]&127);
}

//Remove header and get a list of attributes
vector<string> rmHeader(string SBCPmessage)
{
    vector<string> res;
    int numOfAttr = SBCPmessage[2]*256 + SBCPmessage[3];
    //printf("Number of attr : %d\n",numOfAttr);
    int index = 4;
    for(int i=0; i < numOfAttr; i++){
        int length = 4 + SBCPmessage[index + 2] * 256 + SBCPmessage[index + 3];
        res.push_back(SBCPmessage.substr(index,length));
        index += length;
    }
    return res;
}

//Get the attribute type of a SBCP attribute
AType getAType(string SBCPAttribute)
{
     return (AType)(int)(SBCPAttribute[1]);
}

//Get Payload of a SBCP attribute
string getPayl(string SBCPAttribute)
{
    int length = SBCPAttribute[2] * 256 +SBCPAttribute[3];
    string res="";
    for(int i=4;i<4+length;i++)
        res+= SBCPAttribute[i];
    return res;
}

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
    
    //can't use snprintf cause some char cause terminate earlier
    for(int i = 0; i < message.size(); i++)
		buff[i] = message[i];
    
    return write(sockFd,buff,message.size());		
}

//read
int readSock(int sockFd,string& message )
{
    char buff[MAXLINE];
    
    int n=read(sockFd,buff,MAXLINE);
    //printf("read 1!!\n");
    message = "";
        //Seems we cannot use sprintf to get a string(it will make the string terminate earlier)
    for(int i = 0; i < n; i++)
		message+=buff[i];
    
    return n;
}