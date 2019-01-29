#ifndef __SERVER__H__
#define __SERVER__H__

#define DEFAULT_PORT "8080"
#define ERR_BUFFER_LENGTH 256

#ifdef VERBOSE
    #include <iostream>
#endif

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <errno.h>  // C error numbers for sys libraries
#include <string.h> // strerror_r
#include <unistd.h>
//#include <stdlib.h>

#include <string>
#include <stdexcept>

struct socketSettings 
{
    int addressDomain = AF_INET;
    int socketType = SOCK_STREAM;
    int protocol = 0; // let the OS choose
};

class Server                                                                  
{                                                                                
    private:                                              

        const u_int port;
        
        int socketFd;
        struct socketSettings socketSettings;
        struct sockaddr_in address;

        bool isListening = false;
        uint backlogLength = 10; // arbitrary

        char errBuffer[ERR_BUFFER_LENGTH];

        int createSocket(void);
        struct sockaddr_in bindSocket(void);
        void startListening();

        inline void logTerm(const std::string& str);

        void error(void);

    public:                            

        Server(const std::string userPort = DEFAULT_PORT);
};                                                                               

#endif // __SERVER__H__