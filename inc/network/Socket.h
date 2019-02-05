#ifndef __SOCKET__H__
#define __SOCKET__H__

#include "Exception.h"
#include "ISocket.h"

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

class Socket : public ISocket
{
    public :

        Socket(AddressDomain = IPv4, SocketType = StreamSocket, 
               Protocol = TCP);

        ~Socket();

        int getDescriptor(void) const override;
        ssize_t writeToSocket(std::istream& is) override;
        ssize_t readFromSocket(std::ostream& os) override;
        AddressDomain getAddressDomain(void) const override;
        Protocol getProtocol(void) const override;
        SocketType getSocketType(void) const override;
        void closeSocket(void) override;
        void shutdownSocket(void) override;
        
        Socket() = delete;

    protected : 
    
        int _sd;
        const AddressDomain _domain;
        const SocketType _type;
        const Protocol _protocol;
        bool open = false;

        void except(const std::string& message, 
                    bool showSyserror = true);
};

#endif // __SOCKET__H__