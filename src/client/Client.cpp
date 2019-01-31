#include "Client.h"

Client::Client()
{

}

int Client::openSocket(void)
{
    logTerm("Opening socket... ");

    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (socketFd < 0)
        error("Error opening socket");

    logTerm("[OK]\n");

    return socketFd;
}

struct hostent * Client::resolveHost(const std::string& hostname)
{
    logTerm("Resolving host... ");

    struct hostent * _server = nullptr;
    _server = gethostbyname(hostname.c_str());

    if (_server == NULL)
        error("No such host");

    logTerm("[OK]\n");
    
    return _server;
}

void Client::setServerHost(const std::string& hostname)
{
    // set hostname and server
    struct hostent * _server = resolveHost(hostname);
    struct sockaddr_in _serverAddress = hostAddress(_server);
    
    this->server = _server;
    this->serverName = hostname;
    this->serverAddress = _serverAddress;
}

struct sockaddr_in Client::hostAddress(struct hostent * _server)
{
    struct sockaddr_in _serverAddress;

    bzero((char *) &_serverAddress, sizeof(_serverAddress));
    _serverAddress.sin_family = AF_INET;
    bcopy((char *)_server->h_addr, 
         (char *)&_serverAddress.sin_addr.s_addr,
         _server->h_length);
    _serverAddress.sin_port = htons(port);

    return _serverAddress;
}

void Client::setServerPort(const std::string& port)
{
    setServerPort((uint16_t)std::stoul(port));
}

void Client::setServerPort(const uint16_t port)
{
    (*this).port = port;
}

void Client::establishConnection(int socket)
{
    logTerm("Connecting... ");
    
    if (socket < 0)
        error("Socket not opened");

    if (connect(socket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
            error("Failed to connect");

    logTerm("[OK]\n");
}


void Client::setServer(
    const std::string& hostName, 
    const std::string& portNumber)
{
    setServerPort(portNumber);
    setServerHost(hostName);
}


void Client::talk(const std::string& msg)
{
    std::stringstream log;
    log << "\nSending message: " << std::endl
        << "To: " << inet_ntoa(serverAddress.sin_addr) << std::endl;
    logTerm(log.str());

    int socket = openSocket();
    establishConnection(socket);
    
    logTerm("Writing on socket... ");
    ssize_t bytesWritten = write(
        socket,
        msg.c_str(),
        msg.length()
    );

    if (bytesWritten < 0) 
    {
        close(socket);
        error("Error writing to socket");
    }

    logTerm("[OK]\n");

    logTerm("Reading on socket... ");

    char readBuffer[CLIENT_BUFFER_SIZE];
    bzero(readBuffer, CLIENT_BUFFER_SIZE);
    
    ssize_t bytesRead = read(
        socket, 
        readBuffer, 
        CLIENT_BUFFER_SIZE - 1
    );

    if (bytesRead < 0) 
    {
        close(socket);
        error("Error reading from socket");
    }

    logTerm("[OK]\n");

    std::cout << "Response: " << readBuffer << std::endl;
    

    logTerm("Closing socket... ");
    close(socket);
    logTerm("[OK]\n");

    logTerm("Done!\n");
}


void Client::error(std::string errmsg)
{
    char errBuffer[ERR_BUFFER_LENGTH];
    bzero(errBuffer, ERR_BUFFER_LENGTH);
    strerror_r(errno, errBuffer, ERR_BUFFER_LENGTH);
    
    // prepare error message
    errmsg.append(":");
    errmsg.append(errBuffer);

    throw std::runtime_error(errmsg);
}