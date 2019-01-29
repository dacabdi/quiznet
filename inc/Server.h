#ifndef __SERVER__H__
#define __SERVER__H__

#include <sys/types.h>

class Server                                                                  
{                                                                                
    private:                                              

        Server(void);                                                             
        static Server * _server;
        static bool _initFlag;
                                                                                 
    public:                            

        static Server& getServer(void);                                   
};                                                                               

#endif // __SERVER__H__