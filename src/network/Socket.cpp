#include "Socket.h"

Socket::Socket(AddressDomain domain, SocketType type, Protocol protocol)
: _domain(domain), _type(type), _protocol(protocol)
{
    int sd = socket(domain, type, protocol);
    
    if (sd < 0) 
        throw Exception("Error opening socket", "Socket::Socket:()");

    // avoid address already in use
    int yes=1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) 
        throw Exception(
            "Error setting setting SO_REUSEADDR to 1", 
            "Socket::Socket:()");

    _open = true;
    _sd = sd;
}

Socket::~Socket()
{
    if(_open) closeSocket();
}

int Socket::getDescriptor(void) const
{
    return _sd;
}

ssize_t Socket::writeToSocket(std::istream& is)
{
    std::streamsize ssize = __DATA_BUFFER_WRITE;
    ssize_t total = 0, current = 0;

    char buffer[__DATA_BUFFER_WRITE];
    bzero(buffer, __DATA_BUFFER_WRITE);

    while(!is.eof())
    {
        is.get(buffer, ssize, '\0');
        current = strlen(buffer);
        current = write(_sd, buffer, current);

        if (current < (ssize_t)0) // check if last reading was error (-1)
            throw Exception("Error writing to socket", 
                            "Socket::writeToSocket()", 
                            "Last buffered content ["
                            + std::string(buffer) + "]");

        total += current; // uncount the termination char
    }

    return total;
}

ssize_t Socket::readFromSocket(std::ostream& os, ssize_t nbytes)
{
    ssize_t r = 0;
    int flags = 0;

    char * buffer = new char[nbytes+1];
    bzero(buffer, nbytes+1);
    r = recv(_sd, buffer, nbytes, flags);
    buffer[nbytes+1] = '\0';

    if (r < 0) // check if last reading was error (-1)
    {
        delete [] buffer; // dealloc before excepting
        throw Exception("Error reading from socket", 
                        "Socket::readFromSocket()", 
                        "Last buffered content ["
                        + std::string(buffer) + "]");
    }

    os << buffer;
    delete [] buffer;

    return r;
}

AddressDomain Socket::getAddressDomain(void) const
{
    return _domain;
}

SocketType Socket::getSocketType(void) const
{
    return _type;
}

Protocol Socket::getProtocol(void) const
{
    return _protocol;
}

void Socket::closeSocket(void)
{
    if (close(_sd) != 0)
        throw Exception("Failed to close socket", "Socket::close()");

    _open = false;
    _binded = false;
    _bindedTo = nullptr;
}

void Socket::shutdownSocket(void)
{
    if (shutdown(_sd, SHUT_RDWR) != 0)
        throw Exception("Error shutting down socket", 
                        "Socket::shutdown()");
}

void Socket::bindSocket(const IHost& host)
{
    const struct addrinfo &res = host.getAddressInfo();
    int r = bind(_sd, res.ai_addr, res.ai_addrlen);
    if (r == -1) throw Exception("Failed to bind socket",
                                 "Socket::bindSocket()");
    _binded = true;
    _bindedTo = &host;
}

bool Socket::isBinded(void) const
{
    return _binded;
}

const IHost& Socket::getBindedHost(void) const
{
    return *_bindedTo;
}

void Socket::startListening(int backlog)
{
    if (listen(_sd, backlog) == -1)
        throw Exception("Failed to listen on socket",
                        "Socket::startListening()");
}

void Socket::acceptConnection(void)
{
    struct sockaddr_storage peerAddress;
    socklen_t peerAddressSize = sizeof peerAddress;

    int newSocket = accept(_sd, (struct sockaddr *)&peerAddress, 
                            &peerAddressSize);

    if (newSocket == -1)
       throw Exception("Failed to accept connection on socket",
                       "Socket::acceptConnection()");

    // wrapping the descriptor around will cause
    // the socket to be automatically closed
    // when going out of the scope of this method
    Socket connSocket(newSocket);
    struct sockaddr *temp = ((struct sockaddr *)&peerAddress);
    Host host(temp);

    if(onIncomingConnection)
        onIncomingConnection(connSocket, host, this);
}

void Socket::connectTo(const IHost& host)
{
    const struct addrinfo& res = host.getAddressInfo();
    int c = connect(_sd, res.ai_addr, res.ai_addrlen);

    if (c == -1)
       throw Exception("Failed to connect to host " + host.getAddress(),
                       "Socket::acceptConnection()");

    if(onOutgoingConnection)
        onOutgoingConnection(*this, host, this);
}

Socket::Socket(int sd)
{   
    _open = true;
    _sd = sd;
}