#ifndef __ISOLVEDQUESTION__H__
#define __ISOLVEDQUESTION__H__

#include "Question.h"
#include <string>

/*
    Extends the IQuestion interface to
    a full question that includes a solution.

    This interface is more coherent with the
    stored representation and the data model
    that the server handles.
*/

class IFullQuestion
{
    public:

        virtual char getSolution(void) const = 0;
        virtual const Question& getQuestion(void) const = 0;
        virtual std::string serialize(void) const = 0;

        virtual ~IFullQuestion(){};
};

#endif // __ISOLVEDQUESTION__H__