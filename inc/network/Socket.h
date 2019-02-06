#ifndef __SOCKET__H__
#define __SOCKET__H__

#include "Exception.h"
#include "ISocket.h"
#include "IHost.h"

#include <istream>
#include <ostream>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#ifndef __DATA_BUFFER_READ_SIZE
    #define __DATA_BUFFER_READ_SIZE 1024
#endif

#ifndef __DATA_BUFFER_WRITE_SIZE
    #define __DATA_BUFFER_WRITE_SIZE 1024
#endif

class Socket : public ISocket
{
    public :

        Socket(void) : Socket(IPv4, StreamSocket, TCP) {};

        Socket(AddressDomain addressDomain = IPv4, 
               SocketType socketType = StreamSocket, 
               Protocol protocol = TCP);

        ~Socket();

        int getDescriptor(void) const override;
        ssize_t writeToSocket(std::istream& is) override;
        ssize_t readFromSocket(std::ostream& os) override;
        AddressDomain getAddressDomain(void) const override;
        Protocol getProtocol(void) const override;
        SocketType getSocketType(void) const override;
        void closeSocket(void) override;
        void shutdownSocket(void) override;
        void bindSocket(const IHost& host) override;
        bool isBinded(void) const override;
        const IHost& getBindedHost(void) const override;

    protected : 
    
        int _sd;
        const AddressDomain _domain;
        const SocketType _type;
        const Protocol _protocol;
        bool _open = false;
        bool _binded = false;
        const IHost * _bindedTo = nullptr;
};

#endif // __SOCKET__H__