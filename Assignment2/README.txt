This is the Network Programming Assignment #2 project of ECEN 602, FALL 2016. 

Team 8:  Xiaopei Xu，  Yanxiang Yang

In this project we implemented a TFTP server following the RFC 1350 (without WRQ), include send or operate with RRQ, ACK, ERR and DATA packets.
We have tested this server with tftp_hpa in Ubuntu.

The code file are: tftp.cpp, tftp.h, utils.h.

utils.h contains some utils functions, and defined operation code. It also include all system library used in this project.

tftp.h/cpp contains main functions of the server to be executed.

To compile this project, simply type in 'make' to call the Makefile.Use 'make clean' to clean compiled file.

Implementation:

1. Define a server class to manage the services of TFTP server. Program will create an instance of server class and maintain
    for the whole process;

2. The server will keep receive message, after get a RRQ from client, it will aggregate informations like operation code, file 
    name and client TID, and create a new thread to take care of this transfer; 

3. Each thread for transfer will get a new port number from 32768, and use the information of RRQ to open file and begin to transfer;
    it will also take are of the error when open file;(file size must < 32MB)

4. For transmission, we use 'select' to deal with the timeout. After we send a packet, if we the correct ACK packet, we will transmit 
    next packet, otherwise we will retransmit that packet. After 4 times of retransmission, we will exit this thread and print error;
    
5. After get the ACK for last packet, this thread will end.