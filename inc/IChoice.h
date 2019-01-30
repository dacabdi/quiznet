#ifndef __ICHOICE__H__
#define __ICHOICE__H__

#include <string>

class IChoice
{
    public:
        
        virtual std::string getText() = 0;
        virtual char getId() = 0;
        virtual std::string serialize(void) const = 0;

    protected:

        //IChoice(){};
};

#endif // __ICHOICE__H__