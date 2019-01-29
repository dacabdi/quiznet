#include "Server.h"

Server::Server(const std::string userPort) : port(atoi(userPort.c_str()))
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

void Server::error(void)
{
    bzero(errBuffer, ERR_BUFFER_LENGTH);
    strerror_r(errno, errBuffer, ERR_BUFFER_LENGTH);
    close(socketFd);
    throw std::runtime_error(errBuffer);
}

inline void Server::logTerm(const std::string& str)
{
    #ifdef VERBOSE
        std::cout << str << std::flush;
    #endif
}

void Server::run(void)
{
    while(1)
    {
        // accept next waiting connection, block until some comes in
        struct sockaddr_in clientAddress;
        socklen_t cliendAddressLength = sizeof(clientAddress);

        std::cout << "\n\tWaiting for incoming connection: " << std::endl;
        int connectionSocketFd = accept(
            socketFd, 
            (struct sockaddr *) &clientAddress, 
            &cliendAddressLength
        );
        // ^ accept causes the proc to block until a client
        //   connects to the server


        // rx/tx data
        std::cout << "\tIncoming connection from " 
                << inet_ntoa(clientAddress.sin_addr) 
                << std::endl;

        char *buffer = new char[bufferSize];

        bzero(buffer, bufferSize);
        std::cout << "\tReading message... " << std::flush;
        ssize_t bytesRead = read(connectionSocketFd, buffer, bufferSize - 1);
        if (bytesRead < 0) 
        {
            delete [] buffer;
            close(socketFd);
            error();
        }
        else std::cout << "[OK]" << std::endl;

        std::cout << "\tLength: " << bytesRead << std::endl;
        std::cout << "\tContent: " << buffer << std::endl;

        ssize_t bytesWritten = write(connectionSocketFd, 
            "Hey there, I gotcha!", 
            18
        );

        if (bytesWritten < 0) 
        {
            delete [] buffer;
            close(socketFd);
            error();
        }

        delete [] buffer;

        close(connectionSocketFd);
    }
}