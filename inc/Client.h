#ifndef __CLIENT__H__
#define __CLIENT__H__

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

class Client 
{
    private:
        struct sockarr_in serverAddress;
        struct hostent * server;

        hostent * resolveHost()

        port

    send()
};


#endif __CLIENT__H__