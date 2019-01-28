#include <iostream>     // C++ streams
#include <string.h>     // C string library
#include <unistd.h>     // syscall numbers
#include <stdlib.h>     // exit, etc
#include <stdio.h>      // in/out streams
#include <sys/types.h>  // data types for syscalls
#include <sys/socket.h> // structs for sockets
#include <arpa/inet.h>  // inet_addr to string conversions
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
    std::cout << "Creating socket... " << std::flush;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR: opening socket");
    std::cout << "[OK]" << std::endl;


    // 2. bind socket to an [ip:port] address
    std::cout << "Binding socket... " << std::flush;
    portno = atoi(argv[1]); // read port number from program argument
    bzero((char *) &serv_addr, sizeof(serv_addr)); // initialize buff to 0
    serv_addr.sin_family = AF_INET;                // set address domain
    serv_addr.sin_port = htons(portno);            // convert to NBO
    serv_addr.sin_addr.s_addr = INADDR_ANY;        // any incoming

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        close(sockfd);
        error("ERROR on binding socket to address");
    }
        
    std::cout << "[OK]" << std::endl;


    // 3. start listening for connections
    std::cout << "Listening to connections... " << std::flush;
    listen(sockfd, BACKLOG_SIZE);
    std::cout << "[OK]" << std::endl;

    // 4. accept a connection
    clilen = sizeof(cli_addr); // client address size
    std::cout << "Accepting connections: " << std::endl << std::endl;
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
                // ^ accept causes the proc to block until a client
                //   connects to the server

    // 5. rx/tx data
    std::cout << "\tIncoming connection from " 
              << inet_ntoa(cli_addr.sin_addr) 
              << std::endl;

    bzero(buffer, BUFFER_SIZE);
    std::cout << "\tReading message... " << std::flush;
    n = read(newsockfd, buffer, BUFFER_SIZE - 2);
    if (n < 0) 
    {
        close(sockfd);
        error("ERROR reading from socket!");
    }
    else std::cout << "[OK]" << std::endl;

    std::cout << "\tLength: " << n << std::endl;
    std::cout << "\Content: " << buffer << std::endl;

    n = write(newsockfd, "Hey there, I gotcha!", 18);
    if (n < 0) 
    {
        close(sockfd);
        error("ERROR writing to socket!");
    }

    close(sockfd);

    exit(EXIT_SUCCESS);
}