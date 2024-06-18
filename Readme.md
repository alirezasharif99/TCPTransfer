README - TCP File Transfer
This README provides instructions on how to run and test the TCP file transfer client and server applications.

Requirements
GCC for compiling the C programs.

Files Included
server.c: The TCP server program.
sendFile.c: The TCP client program for sending files.
spawn_clients.sh: A Bash script for spawning multiple client instances.
Makefile: A makefile for compiling the server and client programs.
Reoport.pdf: Contains the testing result for testing the server and client both in one enviroment and two separate enviroments.

Compiling the Programs
Open a terminal.
Navigate to the directory containing the server.c, sendFile.c, and Makefile.
Run make all to compile both the server and client programs.

Running the Server
In the terminal, run ./server <port-number> [bufSize].

Running the Client
In a new terminal, run ./sendFile <fileName> <server-IP-address:port-number> [bufSize].

Spawning Multiple Clients
Make the script executable with chmod +x spawn_clients.sh.
Run the script with ./spawn_clients.sh.

