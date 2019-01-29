#include "Server.h"

Server * Server::_server = nullptr;
bool Server::_initFlag = false;

Server::Server(void)
{
    _initFlag = true;
}

Server& Server::getServer(void)                                              
{                                                                                
    if (!_initFlag)
    {
        _server = new Server();
        return *(_server);
    }
    
    return *(_server);
}