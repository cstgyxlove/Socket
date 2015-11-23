AUTHOR:

NAME: SHITAI CHEN
ID: 1101876651
SESSION:2


DESCRIPTION:

   I built a system which can build a map of the network topology. A set of neighbor information will be distributed among four servers. There will also be a client who is going to initiate the process and also do some computation to give the final topology output. The client and servers are going to communicate with each other to get the neighbor informations from the others and combine all this information together to build the entire map of the network.
   The process is that first the client boost up and build a TCP socket, then it will stay to listen the information from the server. And later the four servers will boost up and read the information in each txt file and transmit it to client. After client receives all information, it will broadcast the information to all servers, let them know the connection.And the client also calculate the MST for the network.
   So this project should contain 4 text files to store the information of the four servers.
The project has three major phases:
1) the client will create a TCP server socket to listen for incoming connections from servers;
2) each node to obtain the entire topology of the network, using the connectivity information of each node.
3) this topology is used to calculate a “tree” or a “minimum spanning tree” that contain all servers.


RUNNING ORDER:
1) Run "make" to compile all the codes. 2) Run "./healthcenterserveroutput" to start TCP server. 3) Run "./doctoroutput" to srart UDP server. 4) Run "./patient1output" and "./patient2output" to start patient process, no specific sequence. 5) Input the index number of time slot in two patient processes, no specific sequence. 6) Type "Ctrl+C" in doctor process to close the doctoroutput. 7) Type "Ctrl+C" in health center process to close the doctoroutput.

