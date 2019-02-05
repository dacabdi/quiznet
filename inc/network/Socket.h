#ifndef __SOCKET__H__
#define __SOCKET__H__

#include "ISocket.h"

#include <istream>
#include <ostream>

// sockets and so on...
#include <sys/socket.h>
#include <netinet/in.h>

// for error handling
#include <errno.h>
#include <string.h>

class Socket : public ISocket
{
    public :

        Socket(AddressDomain = IPv4, SocketType = StreamSocket, 
               Protocol = TCP);

        ~Socket();

        int getDescriptor(void) const override;
        ssize_t write(std::istream& is) override;
        ssize_t read(std::ostream& os) override;
        AddressDomain getAddressDomain(void) const override;
        Protocol getProtocol(void) const override;
        SocketType getSocketType(void) const override;
        void close(void) override;
        void shutdown(void) override;
        
        Socket() = delete;

    protected : 
    
        int _sd;
        const AddressDomain _domain;
        const SocketType _type;
        const Protocol _protocol;

        void except(const std::string& message, 
                    bool showSyserror = true);
};

Socket::Socket(AddressDomain domain, SocketType type, Protocol protocol)
: _domain(domain), _type(type), _protocol(protocol)
{
    int sd = socket(domain, type, protocol);
    if (_sd < 0) except("Socket::Socket:()");
    _sd = sd;
}

Socket::~Socket()
{
    
}

int Socket::getDescriptor(void)
{
    return _sd;
}

SocketType Socket::getAddressDomain(void) const
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

void Socket::close(void)
{

}

void Socket::shutdown(void)
{

}

#endif // __SOCKET__H__