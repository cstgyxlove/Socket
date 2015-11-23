AUTHOR:

NAME: SHITAI CHEN
ID: 1101876651
SESSION:2


WHAT I HAVE DONE:
   I built a system which can build a map of the network topology. A set of neighbor information will be distributed among four servers. There will also be a client who is going to initiate the process and also do some computation to give the final topology output. The client and servers are going to communicate with each other to get the neighbor informations from the others and combine all this information together to build the entire map of the network.
   The process is that first the client boost up and build a TCP socket, then it will stay to listen the information from the server. And later the four servers will boost up and read the information in each txt file and transmit it to client. After client receives all information, it will broadcast the information to all servers, let them know the connection.And the client also calculate the MST for the network.
   So this project should contain 4 text files to store the information of the four servers.
The project has three major phases:
1) the client will create a TCP server socket to listen for incoming connections from servers;
2) each node to obtain the entire topology of the network, using the connectivity information of each node.
3) this topology is used to calculate a “tree” or a “minimum spanning tree” that contain all servers.

WHAT MY CODE FILES ARE:
1) client.c file: this file contains 6 important functions. They are buildTCP(),buildUDPA(),buildUDPB(),buildUDPC(),buildUDPD() and prim().So these functions help client to set up a TCP, and listen the informations from other 4 servers. Then the client send information via UDP to all servers.In the end, the client calculate the MST via function prim()(it also has some other functions to support the function).

2) serverA file: this file contains 2 important functions. They are sendmessage(), receiver(). The first is about sending information to client via TCP, the second is about  receiving information from client via UDP.

3) serverB file: this file contains 2 important functions. They are sendmessage(), receiver(). The first is about sending information to client via TCP, the second is about  receiving information from client via UDP.

4) serverC file: this file contains 2 important functions. They are sendmessage(), receiver(). The first is about sending information to client via TCP, the second is about  receiving information from client via UDP.

5) serverD file: this file contains 2 important functions. They are sendmessage(), receiver(). The first is about sending information to client via TCP, the second is about  receiving information from client via UDP.


RUNNING ORDER:
1) Run “source Makefile.txt“ to compile all the codes.And you should open 5 terminal windows to run
  the program. 2) Run “./client” to start client. 3) Run “./serverA” to start serverA. 4) Run “./serverB” to start serverB.
5) Run “./serverC” to start serverC.
6) Run “./serverD” to start serverD. 7) When you run all the program, the whole process will end.And the answers are on the screen

THE FORMAT OF ALL THE MESSAGES EXCHANGED:
The content of messages are mentioned above, as for the format, I use the char[] buf to store the information and sent.Using the atoi and itoa to change the type.

THE IDIOSYNCRASY:
My program will fail if the content of text file is not like the example given(except the number)


REUSED CODE:
1) Using some code from Beej’s book for the format of build TCP or UDP
2) Using the idea from website: http://blog.csdn.net/piaojun_pj/article/details/5920888
3) Using some code about Prime algorithm from book: 《Algorithm Design -- JON KLEINBERG》 and website:http://blog.csdn.net/qiaoruozhuo/article/details/41515089?utm_source=tuicool&utm_medium=referral




