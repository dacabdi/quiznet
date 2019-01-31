#ifndef __ECHOSERVER__H__
#define __ECHOSERVER__H__

#include "Server.h"
#include "Escape.h"
#include <sstream>
#include <string>

class EchoServer : public Server
{
    protected:

        void onConnection(const int& conn, const sockaddr_in& clientAddress) override;
        ssize_t readConn(const int& conn, char * buffer);
        std::string reply(char * msg);
        void send(const int& conn, const std::string& msg);
    
    public:

        EchoServer(const std::string userPort = DEFAULT_PORT);
};

#endif // __ECHOSERVER__H__