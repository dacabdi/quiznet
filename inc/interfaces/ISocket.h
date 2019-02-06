#ifndef __ISOCKET__H__
#define __ISOCKET__H__

#include "IHost.h"

#include <istream>
#include <ostream>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <functional>

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

class ISocket
{
    public:

        virtual int getDescriptor(void) const = 0;
        virtual ssize_t writeToSocket(std::istream&) = 0;
        virtual ssize_t readFromSocket(std::ostream&) = 0;
        virtual AddressDomain getAddressDomain(void) const = 0;
        virtual Protocol getProtocol(void) const = 0;
        virtual SocketType getSocketType(void) const = 0;
        virtual void closeSocket(void) = 0;
        virtual void shutdownSocket(void) = 0; 
        virtual void bindSocket(const IHost&) = 0;
        virtual bool isBinded(void) const = 0;
        virtual const IHost& getBindedHost(void) const = 0;
        virtual void startListening(int = 5) = 0;
        virtual void acceptConnection(void) = 0;
        virtual void connectTo(const IHost&) = 0;
        
        std::function<void(ISocket&, const IHost&, ISocket*)> 
                                        onIncomingConnection;
        std::function<void(ISocket&, const IHost&, ISocket*)> 
                                        onOutgoingConnection;

        virtual ~ISocket(){};

    protected:

            ISocket(){};
};

#endif // __ISOCKET__H__