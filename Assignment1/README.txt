This is the Network Programming Assignment #1 project of ECEN 602, FALL 2016. 

Team 8:  Xiaopei Xu，  Yanxiang Yang

In this project we finished ALL the bonus features(ipv6, feature1 and feature2).

Code file include utils.h, utils.cpp, client.h, client.cpp, server.h and server.cpp.

Just use command 'make' to compile the code. Use 'make clean' to clean compiled file.


Implementation：

1.  For IPv6, we use some other functions of the socket programming to deal with the address and create socket and bind or connect,
    After that, the program of IPv4 and IPv6 has no difference;

2.  We define two classes to manage the server and client with their information and method. Server has its client count, socket fd, 
    user limit and a hash map of client fd and name. Client has their information of name, socket fd and last active time(for IDLE).
    
    
3.  We use thread to deal with the read and write between clients and server:

    In the server, for each client, the server will create a thread to take care of it, get its message and response(ACK,NAK) or broadcast(ONLINE,
    OFFLINE, IDLE, FWD, etc.)
    
    In each client, a thread is created to get the input from user in command-line, and send the message to server(SEND). Another thread 
    is responsible to read the message from the server and print(ONLINE,OFFLINE,IDLE,FWD, etc.)
    
4. In utils.h/cpp, we create functions to pack attributes and add headers. There are also some functions to send/receive information more easily.

TEST:

Firstly compile the code(call make) and get two file  'Server' and 'Client':

1.  Open a terminal(terminal 1), input    ' ./Server [IP address]  1024  2 ';
    It will put the server online.
    
2.  Open another terminal(terminal 2), input './Client user1 [IP address] 1024';
    This will create a client with user name 'user1', connect and join the server.
    Terminal 2 will show that this client join(JOIN) the terminal and get accepted(ACK), and print the number of users in chat.
    Terminal 1 will show that 'user1 joins the chat.'
    
    Wait 10 seconds, the terminal 2 will show "User user1 is idle."(IDLE)
    
3.  Open terminal 3, input './Client user1 [IP address] 1024';
    It will get rejected (NAK) with the reason "User name is already in use." 
    
    Input './Client user2 [IP address] 1024', then it can join the chat, and other users' name(user1) will print on the screen.
    
    In terminal 2, it will show that "user2 is online" (ONLINE)
    
4.  Open terminal 4, input './Client user3 [IP address] 1024';
    This will be rejected because the server can only serve 2 users(as defined in ' ./Server [IP address]  1024  2')
    
5.  In terminal 2 type 'Hello' and enter, then terminal 3 can get this message.(SEND and FWD).
    Also in terminal 3 type 'Hi' and enter, then terminal 2 can get this message.

6.  Use 'Ctrl + C' to quit the process in terminal 2. Server will know the user1 left and in terminal 3 there is "User user1 is offline."


    From above we can see that this project works well.
    
Terminal 1:
xvxiaopei:~/workspace $ ./Server 172.17.111.165 1024 2
Open server
Server is online
user1 joins chat.
user2 joins chat.
user1 leaves chat.
user2 leaves chat.

Terminal 2:
xvxiaopei:~/workspace $ ./Client user1 172.17.111.165 1024
Open client user1
Send Join...
Get accepted!
Now the server has 1 user(s)
User user1 is idle.
User user2 is online.
User user2 is idle.
Hello
Me: Hello
User user1 is idle.

Terminal 3:
xvxiaopei:~/workspace $ ./Client user1 172.17.111.165 1024
Open client user1
Send Join...
Get rejected!
Reason is : User name is already in use.
xvxiaopei:~/workspace $ ./Client user2 172.17.111.165 1024
Open client user2
Send Join...
Get accepted!
Now the server has 2 user(s)
Other users are: 
  user1
User user2 is idle.
user1: Hello
User user1 is idle.
User user1 is offline.

Terminal 4:
xvxiaopei:~/workspace $ ./Client user4 172.17.111.165 1024                                                                                         
Open client user4
Send Join...
Get rejected!
Reason is : Can't accept more users.


