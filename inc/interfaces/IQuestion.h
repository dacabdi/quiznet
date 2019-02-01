#ifndef __IQUESTION__H__
#define __IQUESTION__H__

#include "Choice.h"
#include "ChoiceCollection.h"
#include "Tag.h"
#include "TagCollection.h"

#include <ostream>
#include <string>
#include <vector>
#include <map>

class IQuestion
{
    public:

        virtual const TagCollection& getTags(void) const = 0;
        virtual const std::string& getQuestion(void) const = 0;
        virtual const ChoiceCollection& getAllChoices(void) const = 0;
        virtual const Choice& getChoiceById(char id) const = 0;                        
        virtual std::string serialize(void) const = 0;

        virtual ~IQuestion(){};

    protected:

        IQuestion(){};
};

#endif // __IQUESTION__H__