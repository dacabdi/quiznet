#ifndef __IADDRESS__H__
#define __IADDRESS__H__

#include "Socket.h"

class IAddress
{
    public:

        virtual int getDescriptor(void) const = 0;
        virtual bool isPassive(void) const = 0;
        virtual std::string getPort(void) const = 0;
        
        virtual ~IAddress(){};

        protected:

            IAddress(){};
};

#endif // __IADDRESS__H__