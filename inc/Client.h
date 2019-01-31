#ifndef __CLIENT__H__
#define __CLIENT__H__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>

#define VERBOSE
#define CLIENT_BUFFER_SIZE 1024
#define ERR_BUFFER_LENGTH 256

class Client 
{
    private:

        std::string serverName;
        struct hostent * server = nullptr;
        struct sockaddr_in serverAddress;
        uint16_t port;

        int openSocket(void);
        struct hostent * resolveHost(const std::string& hostname);
        struct sockaddr_in hostAddress(struct hostent * server);

        void establishConnection(int socket);

        inline void logTerm(const std::string& str) {
            #ifdef VERBOSE
                std::cout << str << std::flush;
            #endif
        };
        
        void setServerHost(const std::string& host);
        void setServerPort(const std::string& port);
        void setServerPort(const uint16_t port);

        void error(std::string errmsg = "");

    public:

        Client();
        void setServer(const std::string& host, const std::string& port);
        void talk(const std::string& msg);
};


#endif // __CLIENT__H__