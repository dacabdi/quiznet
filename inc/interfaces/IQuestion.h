#ifndef __IQUESTION__H__
#define __IQUESTION__H__

/*
    IQuestion defines the interface for a question.
    A question does not neccesarily holds a solution.
    To be semantically specific, it is enough to pose
    a question to be regarded as such.

    See IFullQuestion for a question that incorporates
    a solution.
*/


#include "Choice.h"

#include <ostream>
#include <string>
#include <vector>
#include <map>

class IQuestion
{
    public:

        virtual TagCollection getTags(void) const = 0;
        virtual std::string getQuestion(void) const = 0;
        virtual ChoiceCollection getAllChoices(void) const = 0;
        virtual Choice getChoiceById(char id) const = 0;                        
        virtual std::string serialize(void) const = 0;

        virtual ~IQuestion(){};
};

#endif // __IQUESTION__H__