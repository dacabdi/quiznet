#include "Server.h"

Server::Server(const std::string userPort) 
: port((uint16_t)std::stoul(userPort))
{
    // 1. CREATE SOCKET
    createSocket();
    // 2. BIND SOCKET
    bindSocket();
    // 3. START LISTENING
    startListening();
}

void Server::createSocket(void)
{
    logTerm("Creating socket... ");

    socketFd = socket(
        socketSettings.addressDomain, 
        socketSettings.socketType, 
        socketSettings.protocol
    );
    
    if (socketFd < 0)
        error();

    logTerm("[OK]\n");
}

void Server::bindSocket()
{
    logTerm("Binding socket... ");

    bzero((char *) &address, sizeof(address)); // initialize buff to 0
    address.sin_family = socketSettings.addressDomain; // address domain
    address.sin_port = htons(port);            // conv to net byte order
    address.sin_addr.s_addr = INADDR_ANY;      // any incoming
    
    int result = bind(
        socketFd, 
        (struct sockaddr *)&address, 
        sizeof(address)
    );

    if (result < 0)
        error();
        
    logTerm("[OK]\n");
}

void Server::startListening(void)
{
    logTerm("Listening to connections... ");
    listen(socketFd, backlogLength);
    logTerm("[OK]\n");
}

void Server::acceptConnection(void)
{
    struct sockaddr_in clientAddress;
    socklen_t cliendAddressLength = sizeof(clientAddress);

    logTerm("\n\tWaiting for incoming connection: \n");
    int connectionFd = accept(
        socketFd, 
        (struct sockaddr *) &clientAddress, 
        &cliendAddressLength
    );
    // ^ accept causes the proc to block until a client
    //   connects to the server

    onConnection(connectionFd, clientAddress);
}

void Server::error(void)
{
    bzero(errBuffer, ERR_BUFFER_LENGTH);
    strerror_r(errno, errBuffer, ERR_BUFFER_LENGTH);
    close(socketFd);
    throw std::runtime_error(errBuffer);
}

void Server::run(void)
{
    while(true)
    {
        acceptConnection();
    }
}