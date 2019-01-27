#include <stdlib.h>     // exit, etc
#include <stdio.h>      // in/out streams
#include <sys/types.h>  // data types for syscalls
#include <sys/socket.h> // structs for sockets
#include <netinet/in.h> // constants and structs for internet domain addresses

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
        4. accept a connection with the accept syscall, this is a blocking call
        5. rx/tx data
*/

int main(int argc, char* argv[])
{
    int sockfd,     // file descriptor for socket sys call
        newsockfd,  // file descriptor for the accept sys call
        portno,     // port number accepting connections
        clilen,     // client address size
        n;          // read/write return

    
    

    


    return 0;
}