#include <iostream>     // C++ streams
#include <string.h>     // C string library
#include <unistd.h>     // syscall numbers
#include <stdlib.h>     // exit, etc
#include <stdio.h>      // in/out streams
#include <sys/types.h>  // data types for syscalls
#include <sys/socket.h> // structs for sockets
#include <netinet/in.h> // constants and structs for internet
                        // domain addresses (sockaddr_in)

#define BUFFER_SIZE 256
#define BACKLOG_SIZE 5

// handle and display error
void error(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

/* 
    server setup:

        1. create a socket using the socket syscall
        2. bind the socket using the bind syscall
        3. listen for connections using the listen syscall
        4. accept a connection with the accept syscall, this is 
           a blocking call
        5. rx/tx data
*/

int main(int argc, char* argv[])
{
    int sockfd,     // file descriptor for socket sys call
        newsockfd,  // file descriptor for the accept sys call
        portno;     // port number accepting connections
    
    ssize_t n;        // read/write return
    socklen_t clilen; // client address size

    char buffer[BUFFER_SIZE];

    // a struct containing an internet address ...
    struct sockaddr_in serv_addr, // ... for the server,
                       cli_addr;  // ... for the client

    if (argc < 2)
    {
        std::cerr << "ERROR, no port specified!" << std::endl;
        exit(EXIT_FAILURE);
    }


    // 1. create socket with address domain IPv4 {AF_INET},
    //                       stream socket type (reads as stream) 
    //                       and 0 for the protocol will let the OS choose
    //                       TCP given we asked for a stream socket type
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR: opening socket");



    // 2. bind socket to an [ip:port] address
    portno = atoi(argv[1]); // read port number from program argument
    bzero((char *) &serv_addr, sizeof(serv_addr)); // initialize buff to 0
    serv_addr.sin_family = AF_INET;                // set address domain
    serv_addr.sin_port = htons(portno);            // convert to NBO
    serv_addr.sin_addr.s_addr = INADDR_ANY;        // any incoming

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding socket to address");

    // 3. start listening for connections
    listen(sockfd, BACKLOG_SIZE);

    // 4. accept a connection
    clilen = sizeof(cli_addr); // client address size
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
                // ^ accept causes the proc to block until a client
                //   connects to the server

    // 5. rx/tx data

    bzero(buffer, BUFFER_SIZE);
    n = read(newsockfd, buffer, BUFFER_SIZE - 1);

    if (n < 0) error("ERROR reading from socket!");
    printf("Received message: %s\n", buffer);

    n = write(newsockfd, "Hey there, I gotcha!", 18);
    if (n < 0) error("ERROR writing to socket!");

    return 0;
}