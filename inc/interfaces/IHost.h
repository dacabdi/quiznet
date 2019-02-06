#ifndef __IHOST__H__
#define __IHOST__H__

#include <string>
#include <sys/types.h>
#include <sys/socketvar.h>
#include <netdb.h>

class IHost
{
    public:

        virtual const std::string& getNode(void) const = 0;
        virtual const std::string getAddress(void) const = 0;
        virtual const struct addrinfo& getAddressInfo(void) const = 0; 
        virtual const std::string& getService(void) const = 0;
        virtual bool isPassive(void) const = 0;
        
        virtual ~IHost(){};

    protected:

            IHost(){};
};

#endif // __IHOST__H__