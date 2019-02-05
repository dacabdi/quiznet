#include "Socket.h"

Socket::Socket(AddressDomain domain, SocketType type, Protocol protocol)
: _domain(domain), _type(type), _protocol(protocol)
{
    int sd = socket(domain, type, protocol);
    
    if (sd < 0) 
        throw Exception("Error opening socket", "Socket::Socket:()");

    open = true;
    _sd = sd;
}

Socket::~Socket()
{
    if(open) closeSocket();
}

int Socket::getDescriptor(void) const
{
    return _sd;
}


//TODO: determine if socket has to be open

ssize_t Socket::writeToSocket(std::istream& is)
{
    std::streamsize ssize = __DATA_BUFFER_WRITE_SIZE;
    ssize_t total = 0, current = 0;

    char buffer[__DATA_BUFFER_WRITE_SIZE];
    bzero(buffer, __DATA_BUFFER_WRITE_SIZE);

    while(!is.eof())
    {
        is.get(buffer, ssize);
        current = write(_sd, buffer, ssize);

        if (current < 0) // check if last reading was error (-1)
            throw Exception("Error writing to socket", 
                            "Socket::writeToSocket()", 
                            "Last buffered content ["
                            + std::string(buffer) + "]");

        total += current;
    }

    return total;
}

ssize_t Socket::readFromSocket(std::ostream& os)
{
    ssize_t total = 0, current = 0;
    char buffer[__DATA_BUFFER_READ_SIZE];
    bzero(buffer, __DATA_BUFFER_READ_SIZE);

    while((current = read(_sd, buffer, __DATA_BUFFER_READ_SIZE )) > 0)
    {
        os << buffer;
        total += current;
    }

    if (current < 0) // check if last reading was error (-1)
        throw Exception("Error reading from socket", 
                        "Socket::readFromSocket()", 
                        "Last buffered content ["
                        + std::string(buffer) + "]");

    return total;
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

    open = false;
}

void Socket::shutdownSocket(void)
{
    if (shutdown(_sd, SHUT_RDWR) != 0)
        throw Exception("Error shutting down socket", 
                        "Socket::shutdown()");
}