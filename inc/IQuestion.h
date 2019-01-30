#ifndef __IQUESTION__H__
#define __IQUESTION__H__

#include "Choice.h"

#include <string>
#include <vector>
#include <map>

class IQuestion
{
    public:
        
        virtual uint32_t getId() = 0;
        virtual std::vector<std::string> getTags() = 0;
        virtual std::string getQuestion() = 0;
        virtual std::map<char, Choice> getAllChoices() = 0;
        virtual Choice getChoiceById() = 0;                        
        virtual char getSolution(void) = 0;
        virtual std::string serialize(void) = 0;

    protected:

        IQuestion(){};
};

#endif // __IQUESTION__H__