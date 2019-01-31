#ifndef __SOCKET__H__
#define __SOCKET__H__

#include <sys/types.h>
#include <sys/socket.h>

struct socketSettings 
{
    sa_family_t addressDomain = AF_INET;
    int socketType = SOCK_STREAM;
    int protocol = 0; // let the OS choose
};

#endif