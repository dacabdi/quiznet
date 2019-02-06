#ifndef __ISOCKET__H__
#define __ISOCKET__H__

#include <istream>
#include <ostream>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

// extend for other types

enum SocketType {
    StreamSocket = SOCK_STREAM,
    DatagramSocket = SOCK_DGRAM,
    RawSocket = SOCK_RAW 
};

enum AddressDomain {
    IPv4 = AF_INET,
    IPv6 = AF_INET6
};

enum Protocol {
    DefaultProtocol = 0,
    TCP = IPPROTO_TCP,
    UDP = IPPROTO_UDP
};

#include "IHost.h"

class ISocket
{
    public:

        virtual int getDescriptor(void) const = 0;
        virtual ssize_t writeToSocket(std::istream& is) = 0;
        virtual ssize_t readFromSocket(std::ostream& os) = 0;
        virtual AddressDomain getAddressDomain(void) const = 0;
        virtual Protocol getProtocol(void) const = 0;
        virtual SocketType getSocketType(void) const = 0;
        virtual void closeSocket(void) = 0;
        virtual void shutdownSocket(void) = 0; 
        virtual void bindSocket(const IHost& host) = 0;
        virtual bool isBinded(void) const = 0;
        virtual const IHost& getBindedHost(void) const = 0;
        
        virtual ~ISocket(){};

    protected:

            ISocket(){};
};

#endif // __ISOCKET__H__