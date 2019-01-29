#ifndef __SERVER__H__
#define __SERVER__H__

#define DEFAULT_PORT "8080"
#define ERR_BUFFER_LENGTH 256
#define DEFAULT_READ_BUFFER_LENGTH 1024

#define VERBOSE

#ifdef VERBOSE
    #include <iostream>
#endif

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>  // C error numbers for sys libraries
#include <string.h> // strerror_r
#include <unistd.h>
//#include <stdlib.h>

#include <string>
#include <stdexcept>

struct socketSettings 
{
    sa_family_t addressDomain = AF_INET;
    int socketType = SOCK_STREAM;
    int protocol = 0; // let the OS choose
};

class Server                                                                  
{                                                                                
    protected:                                              

        const uint16_t port;
        
        int socketFd = -1;
        struct socketSettings socketSettings;
        struct sockaddr_in address;

        bool isListening = false;
        uint backlogLength = 10; // arbitrary
        size_t bufferSize = DEFAULT_READ_BUFFER_LENGTH; // arbitrary

        char errBuffer[ERR_BUFFER_LENGTH];

        void createSocket(void);
        void bindSocket(void);
        void startListening(void);
        void acceptConnection(void);

        virtual void onConnection(const int& conn, const sockaddr_in& clientAddress) = 0;

        inline void logTerm(const std::string& str) {
            #ifdef VERBOSE
                std::cout << str << std::flush;
            #endif
        };

        void error(std::string errmsg = "");

    public:                            

        Server(const std::string userPort = DEFAULT_PORT);
        void run();
};                                                                               

#endif // __SERVER__H__