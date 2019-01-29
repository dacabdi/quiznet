#include "Server.h"

Server::Server(const std::string userPort) : port(std::stoul(userPort))
{
    // 1. CREATE SOCKET
    socketFd = createSocket();
    // 2. BIND SOCKET
    address = bindSocket();
    // 3. START LISTENING
    startListening();
}

int Server::createSocket(void)
{
    logTerm("Creating socket... ");

    int _socketFd = socket(
        socketSettings.addressDomain, 
        socketSettings.socketType, 
        socketSettings.protocol
    );
    
    if (_socketFd < 0)
    {
        
    }

    logTerm("[OK]\n");
}

struct sockaddr_in Server::bindSocket()
{
    logTerm("Binding socket... ");

    bzero((char *) &address, sizeof(address)); // initialize buff to 0
    address.sin_family = socketSettings.addressDomain; // address domain
    address.sin_port = htons(port);            // conv to net byte order
    address.sin_addr.s_addr = INADDR_ANY;      // any incoming
    
    if (bind(socketFd, (struct sockaddr*)&address, sizeof(address)) < 0)
        error();
        
    logTerm("[OK]\n");
}

void Server::startListening(void)
{
    logTerm("Listening to connections... ");
    listen(socketFd, backlogLength);
    logTerm("[OK]\n");
}

void Server::error(void)
{
    strerror_r(errno, errBuffer, ERR_BUFFER_LENGTH);
    close(socketFd);
    throw std::runtime_error(errBuffer);
}

inline void logTerm(const std::string& str)
{
    #ifdef VERBOSE
        std::cout << str << std::flush;
    #endif
}