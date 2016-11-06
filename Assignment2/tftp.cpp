/*
    File: tftp.cpp

    Author: Xiaopei Xu
            Department of CSE
            Texas A&M University
    Date  : 09/24/2016

    Description: TFTP server.
*/


#include "tftp.h"


int main(int argc, char** argv){
    printf("Open Server %s\n",argv[1]);
    if(argc != 3)
        err_sys("Number of arguments error\n");
        
    tftp_server server(argv[1],atoi(argv[2]));
}

int tftp_server::sockFd; 
int tftp_server::port_num = 32768;    //port num(TID) start from 32768

//begin TFTP server
tftp_server::tftp_server(char* ip, int port)
{
    struct sockaddr_in addr,client;
    //Create socket(for UDP)
    if((sockFd = socket(AF_INET,SOCK_DGRAM,0))>=0 )    
    { 
         //Initialize addr
        
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(ip);
        addr.sin_port = htons(port);
        
         //Bind
        if (bind(sockFd, (struct sockaddr *) &addr, sizeof(addr) ) < 0)
		    err_sys("bind error");
    }
    else
        err_sys("socket error");
    
    //Online
    while(1){
        char buff[DATA_FIELD];
        memset(&buff, 0, DATA_FIELD);
        int received_bytes = 0;
        socklen_t alen = sizeof(client);
        
        //get the RRQ
        if((received_bytes = recvfrom(sockFd, &buff, DATA_FIELD+4, 0, (struct sockaddr *)&client, &alen))< 0)
			err_sys("accept error\n");
		
		//fill a information struct to create a thread handle this RRQ
		struct info RRQ_info;
		RRQ_info.type = getOp(buff);
        int i=2;
        while(buff[i]!=0)  RRQ_info.file_name+=buff[i++];
        i++;
        while(buff[i]!=0)  RRQ_info.mode+=buff[i++];
        RRQ_info.client_addr = client;
        RRQ_info.server_addr = addr;
        
        printf("Read a RRQ to get file \'%s\'\n",RRQ_info.file_name.c_str());
        
        //create a thread to deal with one RRQ
		pthread_t st;
		pthread_create(&st, NULL, tftp_server::serviceManager, (void *)&RRQ_info);
    }
}

//deal with a RRQ
void * tftp_server::serviceManager(void * RRQ_info)
{
    struct info *client_info  = (struct info*)RRQ_info;
    
    int transFd;
    if((transFd = socket(AF_INET,SOCK_DGRAM,0))<0 )  
        err_sys("socket error");
    
    //pick a new port and bind
    do{
        client_info->server_addr.sin_port = htons(port_num++);
    }
    while(bind(transFd, (struct sockaddr *) &(client_info->server_addr), sizeof(client_info->server_addr) ) < 0);

    //open the file
	ifstream fp;
	fp.open(client_info->file_name.c_str());
    socklen_t alen = sizeof(client_info->client_addr);
    
	// Can't open file
	if(!fp.is_open()){
	    char err[5] = {0, (char)(int)ERROR, 0, (char)(int)FILE_NOT_FOUND, 0};
	    sendto(transFd, err, sizeof(err), 0, (struct sockaddr *) &(client_info->client_addr), alen);
	    printf("File \'%s\' not found.\n",client_info->file_name.c_str());
	    return 0;
	}
	
	//file >=32M
	fp.seekg (0, fp.end);
    if(fp.tellg()>=33554432) {   
        char err[5] = {0, (char)(int)ERROR, 0, (char)(int)ACCESS_DENY, 0};
	    sendto(transFd, err, sizeof(err), 0, (struct sockaddr *) &(client_info->client_addr), alen);
	    printf("File \'%s\' too large.\n",client_info->file_name.c_str());
	    return 0;
    }
	fp.seekg (0, fp.beg);
	
	//Begin to trans packets
	char read_buff[DATA_FIELD];
	char packet[DATA_FIELD+4];
	char receive_buff[DATA_FIELD];
    int block_num = 1;    
    int received_bytes = 0;
    
    while(!fp.eof()){
        memset(&read_buff, 0, DATA_FIELD);
        memset(&read_buff, 0, DATA_FIELD+4);
        fp.read (read_buff,DATA_FIELD);
        // if (fp)
        //     cout << "all characters read successfully.\n";
        // else
        //     cout << "error: only " << fp.gcount() << " could be read\n";
        
        //add header
        packet[1] = (char)(int)DATA;
        packet[2] = block_num/256;
        packet[3] = block_num%256;
        for(int i=4;i<4+fp.gcount();i++)
            packet[i] = read_buff[i-4];
        
        bool ACK = false;
        int retrans = 0;
        fd_set fds;
        //fcntl(transFd, F_SETFL, O_NONBLOCK);
        struct timeval timeout;
        
        //send packet and get ACK
        while(!ACK){
            retrans++;
            if(retrans>=5) {
                printf("Time out for block %d of file \'%s\'\n",block_num,client_info->file_name.c_str());
                return 0;
            }
            sendto(transFd, packet, 4+fp.gcount(), 0, (struct sockaddr *) &(client_info->client_addr), alen);
            printf("Send block %d of file \'%s\'\n",block_num,client_info->file_name.c_str());
           
            
            FD_ZERO(&fds);        
            FD_SET(transFd, &fds);
            timeout.tv_sec = 3;
            timeout.tv_usec = 0;
            select(transFd+1, &fds, NULL, NULL, &timeout);
            if(FD_ISSET(transFd, &fds))
            {
                //get the ACK
                memset(&receive_buff, 0, DATA_FIELD);
                if(recvfrom(transFd, &receive_buff, DATA_FIELD, 0, (struct sockaddr *)&(client_info->client_addr), &alen)< 0)
			        err_sys("accept error\n");
                
                int high = ((int)(receive_buff[2]))&255;
                int low = ((int)(receive_buff[3]))&255;
                
			    //ACK
			    if( getOp(receive_buff)==4 && high*256+low==block_num){
			        printf("Get ACK for block %d of file \'%s\'\n",block_num,client_info->file_name.c_str());
			        ACK = true;
			     } 
            }
        }
        block_num++;
    }
    printf("File \'%s\' transmission complete.\n\n",client_info->file_name.c_str());
	fp.close();

}