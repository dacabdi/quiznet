#ifndef __QUESTION__H__
#define __QUESTION__H__

#include "Choice.h"
#include "ChoiceCollection.h"
#include "Tag.h"
#include "TagCollection.h"
#include "IQuestion.h"
#include <stdexcept>

class Question : public IQuestion
{
    public:

        // create from a string reading id
        Question(const std::string& str);

        // create from stream reading id
        Question(std::stringstream& ss);
        
        // create from pieces
        Question(TagCollection tags,
                 std::string question,
                 ChoiceCollection choices);

        ~Question(){};

        const TagCollection& getTags(void) const override;
        const std::string& getQuestion(void) const override;
        const ChoiceCollection& getAllChoices(void) const override;
        const Choice& getChoiceById(char id) const override;
        std::string serialize(void) const override;

        friend std::ostream& operator<<(
            std::ostream &os, const Question& question);
        virtual bool operator==(const Question& ref);
        virtual bool operator!=(const Question& ref);

    protected:

        TagCollection _tags;
        std::string question;
        ChoiceCollection _choices;

        void init(const std::string& str);
        void init(std::stringstream& ss);
        void init(TagCollection tags,
                  std::string question,
                  ChoiceCollection choices);
};

#endif // __QUESTION__H__